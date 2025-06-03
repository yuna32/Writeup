Dreamhack ptrace_block
=======================


main함수가 ida에서 제대로 디컴파일이 안되어서 풀이를 참고해서 작성했다.


```c
__int64 __fastcall main(int a1, char **a2, char **a3)
{
  int fd; // [rsp+Ch] [rbp-514h]
  __int64 v5[32]; // [rsp+10h] [rbp-510h] BYREF
  char buf[1032]; // [rsp+110h] [rbp-410h] BYREF
  unsigned __int64 v7; // [rsp+518h] [rbp-8h]

  v7 = __readfsqword(0x28u);
  memset(v5, 0, sizeof(v5));
  puts("generate your flag!");
  printf("> ");
  __isoc99_scanf("%255s", v5);
  sub_13F1(v5, buf, 256LL);
  fd = open("./out.txt", 1);
  write(fd, buf, 0x100uLL);
  close(fd);
  return 0LL;
}
```

이게 main() 코드다. 사용자의 입력을 받아 out.txt로 결과를 반환한다. 

문제의 목적은 주어진 out.txt가 나오도록 하는 입력을 알아내는 것이다.


sub_13F1()함수를 본다.


```c
__int64 __fastcall sub_13F1(__int64 a1, __int64 a2, int a3)
{
  char v5[256]; // [rsp+20h] [rbp-120h] BYREF
  __int64 v6[4]; // [rsp+120h] [rbp-20h] BYREF

  v6[3] = __readfsqword(0x28u);
  v6[0] = 0LL;
  v6[1] = 0LL;
  AES_set_encrypt_key(&unk_4010, 128LL, v5);
  AES_cbc_encrypt(a1, a2, a3, v5, v6, 1LL);
  return 0LL;
}
```

주어진 입력을 AES로 암호화하는 것을 확인할 수 있다. 

nk_4010는 AES의 key이고, 
이를 AES_cbc_encrypt()에서 사용하는 형태로 바꾸어 v5에 넣는다. AES_cbc_encrypt()는 암호화를 수행한다. 
v6은 IV이다.

복호화에 key와 IV가 주어졌지만 좀 더 살펴보면

![image](https://github.com/user-attachments/assets/458e97f8-53b1-48d8-9105-c0a1b596259f)

init array에 함수들이 더 있음을 발견할 수 있다. **sub_12C9**와 **sub_1392**


![image](https://github.com/user-attachments/assets/39697d60-b7ca-4c65-b20b-f649ff55730d)

코드를 분석하면

1.  **난수 생성기 초기화**


`time(0LL)`을 시드로 사용하여 `srand()`를 호출한다. 
매번 프로그램이 실행될 때마다 다른 난수 시퀀스를 생성하도록 한다.

2.  **안티 디버깅 메커니즘**
    * 첫 번째 `for` 루프에서 4096번(`4095 - 0 + 1`) `ptrace(PTRACE_TRACEME, 0, 0, 0)` 호출을 반복한다.
    * `PTRACE_TRACEME`는 호출하는 프로세스가 자신을 추적하도록 요청하는 기능이다.
    * **정상적인 실행 (디버거 없음):** 이 호출은 성공하며 `v1`에 0을 반환한다. 결과적으로 `v2 *= 0 * rand()`는 `v2`를 계속 0으로 만든다. 이 루프가 끝난 후 `v2`는 0이 된다.
    * **디버깅 중 (디버거 있음):** 프로세스가 이미 디버거에 의해 추적되고 있다면 `ptrace(PTRACE_TRACEME, ...)` 호출은 실패하고 `v1`에 -1을 반환한다. 이 경우 `v2 *= (-1) * rand()` 연산이 수행되며 `v2`는 반복적으로 음수 값을 가지게 된다.

3.  **`byte_4010` 배열 값 조작:**
    * 두 번째 `for` 루프는 `byte_4010`이라는 전역 바이트 배열의 내용을 수정한다.
    * `byte_4010[j + 1] += byte_4010[j] + v2;` 연산은 이전 바이트(`byte_4010[j]`)와 현재 바이트(`byte_4010[j+1]`)의 값을 더하고, 여기에 **첫 번째 루프에서 결정된 `v2`의 값**을 더하여 새로운 `byte_4010[j+1]` 값을 만든다.
  

![image](https://github.com/user-attachments/assets/6234e948-c1fe-4c2b-a64b-c688428bbf55)


sub_1392 함수는 byte_4010이라는 전역 변수 배열의 처음 16바이트를 v3 (단일 바이트 난수) 값으로 XOR 연산한다.


out.txt에 저장된 암호화된 데이터를 복호화하려면 byte_4010[i]과 rand()값을 xor한 값을 브루트포스해서 알아낼 수 있다.

형식이 DH{...}니까

```python
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>
#include <pthread.h>
#include <openssl/aes.h>
#include <sys/ptrace.h>
#include <sys/fcntl.h>

#define KEY_BIT 128

int aes_decrypt(uint8_t *p_in, uint8_t *p_out, uint8_t *cipher_key, int size)
{
    AES_KEY aes_key;
    uint8_t cipher_iv[AES_BLOCK_SIZE] = {0, };

    AES_set_decrypt_key(cipher_key, KEY_BIT, &aes_key);
    AES_cbc_encrypt(p_in, p_out, size, &aes_key, cipher_iv, AES_DECRYPT);
    return 0;
}

uint8_t cipher_key[AES_BLOCK_SIZE] = {65, 40, 25, 78, 165, 124, 161, 65, 19, 207, 136, 172, 42, 240, 183, 218};


int main()
{
    uint8_t p_decrypt[0x100];
    uint8_t p_temp[1024];

    int fd = open("./out.txt", O_RDONLY);
    read(fd, p_decrypt, 0x100);

    for(int j=0; j<0x100; j++)
    {
        uint8_t cipher_key_cpy[0x10];
        memcpy(cipher_key_cpy, cipher_key, 0x10);
        for(int i=0; i<15; i++)
            cipher_key_cpy[i+1] += cipher_key_cpy[i];
        for(int i=0; i<16; i++)
            cipher_key_cpy[i] ^= j;

        aes_decrypt(p_decrypt, p_temp, cipher_key_cpy, 0x100);
        if(!strncmp(p_temp, "DH", 2))
            printf("%s", p_temp);
    }
}

```

코드의 주요 부분을 보면


**키 변형 로직**
main 함수 내의 for (int j=0; j<0x100; j++) 루프가 중요하다. 
이 루프는 j 값을 0부터 255까지 증가시키면서 매번 복호화 키를 변형한다.

* 단계 1: **누적 합산 변형 (cipher_key_cpy[i+1] += cipher_key_cpy[i];)** 전역 cipher_key를 복사한 cipher_key_cpy에 대해, 각 바이트에 이전 바이트의 값을 누적해서 더하는 연산을 수행한다.
 이는 키의 각 바이트가 이전 바이트의 영향을 받게 만든다.
* 단계 2: **j 값과의 XOR 변형 (cipher_key_cpy[i] ^= j;)** 단계 1에서 변형된 cipher_key_cpy의 모든 16바이트를
 현재 루프 변수 j 값과 XOR 연산한다. 이 j 값이 0부터 255까지 변하므로, 총 256가지의 다른 키가 생성된다.


**평문 검증 (크랙/복구 조건)**
각각의 변형된 키로 암호문을 복호화한 후, 결과 평문 p_temp가 "DH"라는 두 글자로 시작하는지 strncmp를 사용하여 검사한다. 
만약 "DH"로 시작하면 해당 p_temp의 내용을 표준 출력(printf)한다.


![image](https://github.com/user-attachments/assets/13dfdec5-9626-46fd-836b-56aae62835df)

컴파일하고 실행하여 플래그를 얻어낸다.
