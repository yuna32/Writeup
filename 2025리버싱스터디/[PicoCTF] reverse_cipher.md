[PicoCTF] reverse_cipher
=======================

![image](https://github.com/user-attachments/assets/4dd0b7c9-cee5-4a82-97c7-ce2910cd3d3e)

주어진 파일 중 rev는 elf 파일, rev_this는 암호화된 플래그를 담은 텍스트 파일이다.


rev 파일을 ida에서 열어 디컴파일해보면

```c
int __fastcall main(int argc, const char **argv, const char **envp)
{
  _BYTE ptr[23]; // [rsp+0h] [rbp-50h] BYREF
  char v5; // [rsp+17h] [rbp-39h]
  int v6; // [rsp+2Ch] [rbp-24h]
  FILE *v7; // [rsp+30h] [rbp-20h]
  FILE *stream; // [rsp+38h] [rbp-18h]
  int j; // [rsp+44h] [rbp-Ch]
  int i; // [rsp+48h] [rbp-8h]
  char v11; // [rsp+4Fh] [rbp-1h]
  stream = fopen("flag.txt", "r");
  v7 = fopen("rev_this", "a");
  if ( !stream )
    puts("No flag found, please make sure this is run on the server");
  if ( !v7 )
    puts("please run this on the server");
  v6 = fread(ptr, 0x18uLL, 1uLL, stream);
  if ( v6 <= 0 )
    exit(0);
  for ( i = 0; i <= 7; ++i )
  {
    v11 = ptr[i];
    fputc(v11, v7);
  }
  for ( j = 8; j <= 22; ++j )
  {
    v11 = ptr[j];
    if ( (j & 1) != 0 )
      v11 -= 2;
    else
      v11 += 5;
    fputc(v11, v7);
  }
  v11 = v5;
  fputc(v5, v7);
  fclose(v7);
  return fclose(stream);
}
```


가장 중요한 암호화 부분을 보면

* `fread(ptr, 0x18uLL, 1uLL, stream)`을 사용하여 flag.txt에서 24바이트(`0x18uLL`)의 데이터를 
    `ptr` 배열로 읽어온다.
* **첫 8바이트 (인덱스 0부터 7까지)**: flag.txt에서 읽어온 처음 8바이트는 **아무런 변경 없이 그대로 `rev_this` 파일에 기록**된다.
* **다음 15바이트 (인덱스 8부터 22까지)**: 이 부분의 데이터에 대해 암호화가 수행된다.
    * **홀수 인덱스(`j & 1) != 0`)**: 해당 바이트의 값에서 2를 뺀다 (v11 -= 2).
    * **짝수 인덱스**: 해당 바이트의 값에 5를 더한다 (v11 += 5).
    * 변환된 값은 rev_this 파일에 기록된다.
* **마지막 1바이트**: `v5`라는 변수의 값이 rev_this 에 기록된다. 이 `v5` 변수는 초기화되지 않은 지역 변수로, 실제로는 `ptr` 배열의 23번째(`0x17h`) 바이트를 나타낸다. 
    따라서 `ptr[23]` (실제로는 `ptr` 배열의 범위를 벗어난 위치)에 저장된 값, 
    혹은 쓰레기 값이 기록될 수 있다.


따라서 복호화 코드를 작성해보면


```python
import os

def decrypt_rev_this_from_file(file_path):

    try:
        with open(file_path, 'rb') as encrypted_file:
            encrypted_data = encrypted_file.read()
    except Exception as e:
        print(f"파일을 읽는 중 오류 발생: {e}")
        return

    decrypted_bytes = bytearray()

    # 첫 8바이트는 변경되지 않음
    # 파일 크기가 8바이트 미만일 경우 대비하여 min() 사용
    for i in range(min(8, len(encrypted_data))):
        decrypted_bytes.append(encrypted_data[i])

    # 인덱스 8부터 22까지의 바이트를 복호화
    # 원래 암호화 로직은
    # 홀수 인덱스 (j & 1) != 0 : v11 -= 2  -> 복호화: v11 += 2
    # 짝수 인덱스 (else)        : v11 += 5  -> 복호화: v11 -= 5
    for j in range(8, min(23, len(encrypted_data))): # 23은 인덱스 22까지 포함
        char_code = encrypted_data[j]
        if (j % 2) != 0:  # 홀수 인덱스
            decrypted_char_code = char_code + 2
        else:  # 짝수 인덱스
            decrypted_char_code = char_code - 5
        decrypted_bytes.append(decrypted_char_code)

    # 원본에서 v5 (ptr[23] 또는 쓰레기 값)가 마지막에 추가될 수 있다
    # 파일 크기가 23바이트보다 크다면, 그 이후의 모든 바이트는 그대로 복사
    if len(encrypted_data) > 23:
        for k in range(23, len(encrypted_data)):
            decrypted_bytes.append(encrypted_data[k])

    # 복호화된 바이트열을 문자열로 디코딩하여 출력
    try:
        print(f"복호화된 결과: {decrypted_bytes.decode('ascii')}")
    except Exception:
        pass 


file_to_decrypt = r"C:\Users\gram\Downloads\rev_this"

if __name__ == "__main__":
    decrypt_rev_this_from_file(file_to_decrypt)
```


![image](https://github.com/user-attachments/assets/6c002192-5990-43e7-9c17-f0ccab264ecd)




