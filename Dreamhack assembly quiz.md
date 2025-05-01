Quiz 1
==========




Question 1
-----------
![image](https://github.com/user-attachments/assets/e16126e0-c41a-4096-9f17-50caa0dea089)





**`mov rax, [rbx+8]`** 에 주목해본다.

rbx 레지스터의 현재 값은 '0x401A40' 이다. rbx+8은 메모리 주소 0x401A40 + 8 = 0x401A48을 나타낸다.

[rbx+8]은 메모리 주소 0x401A48에 저장된 값을 의미한다.
메모리 내용을 보면 `0x401A48`에는 `0x0000000000C0FFEE`가 저장되어 있다.
따라서 mov rax, [rbx+8] 명령어는 메모리 주소 0x401A48의 값을 rax 레지스터로 복사한다고 볼 수 있다.

결과적으로 Code를 1까지 실행한 후 `rax` 레지스터의 값은 **0xC0FFEE** 가 된다.




Question 2
-------------

![image](https://github.com/user-attachments/assets/095b0a2b-3629-45ea-aea4-b8470dafa094)



먼저 mov rax, [rbx+8]를 보자.



1번 문제와 똑같이, rbx 레지스터의 값은 0x401A40 이다.
rbx+8은 메모리 주소 0x401A40 + 8 = 0x401A48을 나타낸다.
[rbx+8]은 메모리 주소 0x401A48에 저장된 값 0x0000000000C0FFEE를 나타낸다.
따라서 rax 레지스터에는 0x0000000000C0FFEE가 저장된다.


이제 Code 2, **lea rax, [rbx+8]** 를 보자.

lea 명령어는 메모리 주소의 값을 읽어오는 것이 아니라 유효 주소를 계산하여 레지스터에 저장한다.
rbx 레지스터의 값은 여전히 0x401A40 이다.
rbx+8은 메모리 주소 0x401A40 + 8 = 0x401A48을 나타낸다.
따라서 lea rax, [rbx+8] 명령어는 메모리 주소 0x401A48 자체를 rax 레지스터에 저장한다.


결과적으로 Code를 2까지 실행한 후 rax 레지스터의 값은 **0x401A48**이 된다. 


Question 3
--------------

![image](https://github.com/user-attachments/assets/50c94bbf-6fd2-4efc-b716-0458ac63bc1b)

**add rax, [rbx+rcx*8]** 을 생각하는 문제다.

현재 rax 레지스터의 값은 0x31337 이다.
rbx 레지스터의 값은 0x555555554000 이다.
rcx 레지스터의 값은 0x2 이다.
rcx * 8은 0x2 * 8 = 0x10 이다.


rbx + rcx * 8은 메모리 주소 0x555555554000 + 0x10 = 0x555555554010을 나타낸다.

[rbx+rcx*8]은 메모리 주소 0x555555554010에 저장된 값을 의미하며 이는 0x0000000000000003 이다.

add rax, [rbx+rcx*8] 명령어는 rax의 현재 값 (0x31337)에 메모리 주소 0x555555554010의 값 (0x3)을 더한다.

**0x31337 + 0x3 = 0x3133A**


따라서 Code를 1까지 실행한 후 rax 레지스터의 값은 0x3133A가 된다.



Question 4
------------------

![image](https://github.com/user-attachments/assets/ff8be66c-1b39-4344-86e4-a7487bec6241)


**add rax, [rbx+rcx*8] (Code 1 실행 후)**

rax = 0x31337,
rbx = 0x555555554000,
rcx = 0x2


rbx + rcx * 8 = 0x555555554000 + (0x2 * 0x8) = 0x555555554000 + 0x10 = 0x555555554010
[rbx+rcx*8] (메모리 주소 0x555555554010의 값) = 0x0000000000000003 (즉 0x3)
rax = 0x31337 + 0x3 = 0x3133A


**add rcx, 2 (Code 2 실행 후)**

rcx = 0x2 + 0x2 = 0x4



**sub rax, [rbx+rcx*8] (Code 3 실행 후)**

현재 rax = 0x3133A,
rbx = 0x555555554000,
현재 rcx = 0x4


rbx + rcx * 8 = 0x555555554000 + (0x4 * 0x8) = 0x555555554000 + 0x20 = 0x555555554020

[rbx+rcx*8] (메모리 주소 0x555555554020의 값) = 0x000000000003133A (간단히 0x3133A)

rax = 0x3133A - 0x3133A = 0x0

따라서 Code를 3까지 실행했을 때, rax 레지스터에 저장된 값은 **0x0 즉 0** 이다.


Question 5
---------------

![image](https://github.com/user-attachments/assets/e5495b9f-eda5-45dc-af03-2b4e9e4f53af)


일단 code 3까지는 4번 문제와 같으니 넘어간다.

**inc rax (Code 4 실행 후)**

현재 rax = 0x0
inc rax 명령어는 rax의 값을 1 증가시킨다.
rax = 0x0 + 1 = 0x1

따라서 Code를 4까지 실행했을 때, rax 레지스터에 저장된 값은 **0x1 즉 1** 이다.


Question 6
-----------------

![image](https://github.com/user-attachments/assets/b4bc1c7c-74f4-454e-8794-cf21e8070bbc)

**and rax, rcx** 관련 문제다.

and 명령어는 rax 레지스터의 값과 rcx 레지스터의 값에 대해 비트 AND 연산을 수행하고 그 결과를 rax 레지스터에 저장한다.
rax의 현재 값은 0xffffffff00000000 이다.
rcx의 현재 값은 0x123456789abcdef0 이다.


* `0xf & 0x1 = 0x1`
* `0xf & 0x2 = 0x2`
* `0xf & 0x3 = 0x3`
* `0xf & 0x4 = 0x4`
* `0xf & 0x5 = 0x5`
* `0xf & 0x6 = 0x6`
* `0xf & 0x7 = 0x7`
* `0xf & 0x8 = 0x8`
* `0x0 & 0x9 = 0x0`
* `0x0 & 0xa = 0x0`
* `0x0 & 0xb = 0x0`
* `0x0 & 0xc = 0x0`
* `0x0 & 0xd = 0x0`
* `0x0 & 0xe = 0x0`
* `0x0 & 0xf = 0x0`
* `0x0 & 0x0 = 0x0`

비트 AND 연산을 수행하면 **0x1234567800000000** 이다.


Question 7
---------------

![image](https://github.com/user-attachments/assets/c86cb6ca-76a0-4d45-8f65-af468bf0bf0c)

일단 code 1까지는 6번 문제와 같으므로 
초기 rbx 값: 0x00000000ffffffff,
rcx 값: 0x123456789abcdef0


and rbx, rcx 명령어는 rbx 레지스터의 값과 rcx 레지스터의 값에 대해 비트 AND 연산을 수행하고 그 결과를 rbx 레지스터에 저장한다.

**최상위 8자리 (`0xffffffff` AND `0x12345678`):**
* `0xf` (1111) AND `0x1` (0001) = `0x1` (0001)
* `0xf` (1111) AND `0x2` (0010) = `0x2` (0010)
* `0xf` (1111) AND `0x3` (0011) = `0x3` (0011)
* `0xf` (1111) AND `0x4` (0100) = `0x4` (0100)
* `0xf` (1111) AND `0x5` (0101) = `0x5` (0101)
* `0xf` (1111) AND `0x6` (0110) = `0x6` (0110)
* `0xf` (1111) AND `0x7` (0111) = `0x7` (0111)
* `0xf` (1111) AND `0x8` (1000) = `0x8` (1000)

결과: `0x12345678`

**최하위 8자리 (`0x00000000` AND `0x9abcdef0`):**
* `0x0` (0000) AND `0x9` (1001) = `0x0` (0000)
* `0x0` (0000) AND `0xa` (1010) = `0x0` (0000)
* `0x0` (0000) AND `0xb` (1011) = `0x0` (0000)
* `0x0` (0000) AND `0xc` (1100) = `0x0` (0000)
* `0x0` (0000) AND `0xd` (1101) = `0x0` (0000)
* `0x0` (0000) AND `0xe` (1110) = `0x0` (0000)
* `0x0` (0000) AND `0xf` (1111) = `0x0` (0000)
* `0x0` (0000) AND `0x0` (0000) = `0x0` (0000)

결과: `0x00000000`

두 부분을 합치면 0x00000000과 0x9abcdef0이 연결되어 0x000000009abcdef0 이 된다.

따라서 Code 2 (and rbx, rcx)를 적용한 이후의 rbx 레지스터 값은 **0x000000009abcdef0** 이다.


Question 8
--------------

![image](https://github.com/user-attachments/assets/9ad464b6-a3b5-45f8-8006-6aeebede31ed)

Code 1 (and rax, rcx)의 결과로 rax 레지스터에는 0x1234567800000000이 저장되었다.

Code 2 (and rbx, rcx)의 결과로 rbx 레지스터에는 0x000000009abcdef0이 저장되었다.

이제 Code 3 (or rax, rbx)를 적용한 이후의 rax 레지스터 값을 계산해보자.

or rax, rbx 명령어는 rax 레지스터의 값과 rbx 레지스터의 값에 대해 비트 OR 연산을 수행하고 그 결과를 rax 레지스터에 저장한다.


**최상위 8자리:**
* `0x1` OR `0x0` = `0x1`
* `0x2` OR `0x0` = `0x2`
* `0x3` OR `0x0` = `0x3`
* `0x4` OR `0x0` = `0x4`
* `0x5` OR `0x0` = `0x5`
* `0x6` OR `0x0` = `0x6`
* `0x7` OR `0x0` = `0x7`
* `0x8` OR `0x0` = `0x8`

결과: `0x12345678`

**최하위 8자리:**
* `0x0` OR `0x9` = `0x9`
* `0x0` OR `0xa` = `0xa`
* `0x0` OR `0xb` = `0xb`
* `0x0` OR `0xc` = `0xc`
* `0x0` OR `0xd` = `0xd`
* `0x0` OR `0xe` = `0xe`
* `0x0` OR `0xf` = `0xf`
* `0x0` OR `0x0` = `0x0`

결과: `0x9abcdef0`


두 부분을 합치면 0x12345678과 0x9abcdef0이 연결되어 0x123456789abcdef0 이 된다.

따라서 Code 3 (or rax, rbx)를 적용한 이후의 rax 레지스터 값은 **0x123456789abcdef0** 이다.



Question 9
--------------

![image](https://github.com/user-attachments/assets/c7319a53-6f8d-4671-81ae-4cf2b2395e8e)

**xor rax, rbx** 관련 문제다.

xor 명령어는 rax 레지스터의 값과 rbx 레지스터의 값에 대해 비트 XOR 연산을 수행하고 그 결과를 rax 레지스터에 저장한다.

rax의 현재 값은 0x35014541 이다.
rbx의 현재 값은 0xdeadbeef 이다.


비트 XOR 연산은 두 비트가 서로 다를 때 결과 비트는 1이 되고, 같을 때는 0이 되는 연산이다.

```

rax = 0x35014541 = 0011 0101 0000 0001 0100 0101 0100 0001
rbx = 0xdeadbeef = 1101 1110 1010 1101 1011 1110 1110 1111
---------------------------------------------------------
XOR =              1110 1011 1010 1100 1111 1011 1010 1110 = 0xebacfbae

```

즉 0x35014541 XOR 0xdeadbeef = 0xebacfbae

결과: `0xebacfbae`



Question 10
---------------
![image](https://github.com/user-attachments/assets/b8035eb0-cbdc-4eb4-beee-ff5bbc3e4cc8)


Cxor rax, rbx 한 번 더 실행하라는 문제이다.

그런데 xor 연산은 역연산이 가능해서 (A ^ B = C → C ^ B = A 와 같이) Code 2 → 다시 **rax = 0x35014541**로 돌아간다.



Question 11
------------

![image](https://github.com/user-attachments/assets/b63e19e1-26d9-4c80-bd4d-5099077b1175)

Code 3는 not eax이다.
not은 비트 반전 연산이다. 

eax는 rax의 하위 32비트이므로 eax = 0x35014541이다.
eax에 not 연산 적용하면


```
eax = 0x35014541 = 0011 0101 0000 0001 0100 0101 0100 0001
NOT =              1100 1010 1111 1110 1011 1010 1011 1110 
```


2진수 → 16진수로 다시 변환

11001010 = 0xCA  
11111110 = 0xFE  
10111001 = 0xBA 
10111110 = 0xBE

따라서 **`~0x35014641` = `0xCAFEBABE`**





Quiz 2
===========

Question 1
--------------

![image](https://github.com/user-attachments/assets/89414845-c25b-4cb2-9b4c-bc04db1eaee8)




- **rsi = 0x400000** (디코딩할 메모리 시작 주소)
- **rcx = 0**, **rdx = 0**


코드를 관찰하면


**1: mov dl, BYTE PTR[rsi+rcx]**

rsi + rcx는 메모리 주소를 계산한다. rsi는 시작 주소이고, rcx는 오프셋이다.

BYTE PTR은 해당 메모리 주소에서 1바이트(8비트) 데이터를 읽어온다.
읽어온 바이트 데이터는 dl 레지스터에 저장된다.

**2: xor dl, 0x30**

dl 레지스터의 값과 16진수 값 0x30 (십진수 48)을 비트 XOR 연산한다.
XOR 연산의 결과는 다시 dl 레지스터에 저장된다.

**3: mov BYTE PTR[rsi+rcx], dl**

dl 레지스터의 현재 값을 rsi + rcx가 가리키는 메모리 주소에 1바이트로 저장한다. 
즉 원래 메모리 내용을 XOR 연산 결과로 덮어쓴다.


**4: inc rcx**

rcx 레지스터의 값을 1 증가시킨다.

**5: cmp rcx, 0x19:**

rcx 레지스터의 값과 16진수 값 0x19 (십진수 25)를 비교합니다.

**6: jg end**

rcx의 값이 0x19보다 크면 end로 점프하여 프로그램을 종료한다.


**7: jmp 1**

조건 없이 코드의 1번 레이블로 점프한다. 이는 코드를 다시 처음부터 실행하도록 만든다.



**원본 메모리 (0x400000 ~ 0x400018, 총 25바이트)**

```
0x400000 | 0x67 0x55 0x5c 0x53 0x5f 0x5d 0x55 0x10
0x400008 | 0x44 0x5f 0x10 0x51 0x43 0x43 0x55 0x5d
0x400010 | 0x52 0x5c 0x49 0x10 0x47 0x5f 0x42 0x5c
0x400018 | 0x54 
```

**XOR 0x30 적용**

하나씩 xor 하기에는 번거로워서 파이썬 코드를 작성해 사용했다. 

```python
data = [
    0x67, 0x55, 0x5c, 0x53, 0x5f, 0x5d, 0x55, 0x10,
    0x44, 0x5f, 0x10, 0x51, 0x43, 0x43, 0x55, 0x5d,
    0x52, 0x5c, 0x49, 0x10, 0x47, 0x5f, 0x42, 0x5c,
    0x54
]

# XOR 연산 후 문자열 디코딩
decoded = [byte ^ 0x30 for byte in data]
ascii_string = ''.join(chr(b) for b in decoded)

print( ascii_string)
```

이 코드를 돌려보면

![image](https://github.com/user-attachments/assets/d85889df-193d-4bf0-94dd-3fe884854b2a)

따라서 정답은 **Welcome to assembly world** 라 보았다.

그런데 이게 답이 아니라 해서 0x54의 다음 바이트인 0x11 (== 17)을 xor 0x30 시켜보면 !(느낌표) 가 된다.

그래서 느낌표까지 포함한 **Welcome to assembly world!** 를 입력하니 답으로 나온다. 이게 최종 정답이라 할 수 있다.



Quiz 3
================

Question 1
--------------

![image](https://github.com/user-attachments/assets/fade4eac-ff44-45cb-bc93-522ba45c2218)
![image](https://github.com/user-attachments/assets/8c579a9e-25a8-47b1-ae70-c2c194ac87bf)

**`main` 함수의 동작** 을 먼저 관찰해본다. 주요 부분은 다음과 같다.

```asm
mov esi, 0xf         ; 길이 = 15 (10진수)
mov rdi, 0x400500    ; 버퍼 주소 = 0x400500
call write_n         ; write_n 호출
```
 - mov esi, 0xf: 값 0xf (십진수 15)를 esi 레지스터에 복사한다. 
- mov rdi, 0x400500: 메모리 주소 0x400500을 rdi 레지스터에 복사한다. 
- call 0x400497 <write_n>: 주소 0x400497에 위치한 write_n 함수를 호출한다. rdi와 esi 레지스터에 설정된 값들이 write_n 함수의 인자로 전달된다.

이제 **`write_n` 함수** 를 분석해본다.

```asm
write_n:
    push rbp
    mov rbp, rsp

    mov [rbp-0x8], rdi  ; buf
    mov [rbp-0xc], esi  ; size = 0xf = 15

    xor rdx, rdx
    mov edx, [rbp-0xc]  ; rdx = count = 15
    mov rsi, [rbp-0x8]  ; rsi = buffer = 0x400500
    mov rdi, 0x1        ; fd = 1 
    mov rax, 0x1        ; syscall number = 1 (write)
    syscall             ; write(1, rsi, rdx)
```

결국 핵심 부분은 syscall로 보인다. syscall은 Linux 시스템 호출을 수행하여 rax에 지정된 시스템 호출 (write), 
rdi에 지정된 파일 디스크립터(표준 출력: 1), 
rsi에 지정된 버퍼 주소 (0x400500), rdx에 지정된 바이트 수 (0xf)를 사용하여 동작한다. 

즉 syscall을 통해서

```c
write(1, (char*)0x400500, 15);
```

를 실행하는 함수이다.


이제 메모리에 저장된 16바이트 값을 리틀 엔디안으로 변환하여 바이트 배열로 추출하고, 
앞에서 15바이트만 출력해야 한다. (write syscall이 15바이트만 출력하기 때문이다)

이를 위해서 파이썬 코드를 작성했다. 변환에서 좀 어려움을 겪어서 지피티를 참고했다.

```python
import struct

# 메모리에 저장된 16바이트 값 (0x400500~0x40050F까지)
mem = [
    0x3037207964343372,  # 0x400500
    0x003f367562336420   # 0x400508
]

# 리틀 엔디안으로 변환, 바이트 배열 추출
raw_bytes = b''.join(struct.pack('<Q', val) for val in mem)

# 앞에서 15바이트 출력 
decoded = raw_bytes[:15].decode('ascii')

print(decoded)
```
- `struct.pack('<Q', val)` → 8바이트 정수(`Q`)를 **리틀 엔디안(`<`)**으로 바이트로 변환.
- `.join(...)` → 두 개의 8바이트 메모리를 연결.
- `[:15]` → `0xf = 15바이트`만 추출.
- `.decode('ascii')` → 바이트를 문자열로 디코딩




![image](https://github.com/user-attachments/assets/f4dbc75a-f6a7-48c0-8aa7-6ab8bf0225a3)

답은 **r34dy 07d23bu6?** 이다. 
