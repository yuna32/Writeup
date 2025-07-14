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

```cpp
strncpy(
    static_cast<UndiscoveredMineral*>(minerals[index])->description,
    description.c_str(),
    MAX_DESCRIPTION_SIZE
);
```

* Mineral 클래스는 추상 클래스 (virtual void print_description())
* UndiscoveredMineral은 char description[MAX_DESCRIPTION_SIZE]를 가지고 있으며 이 description을 출력함.
* RareEarth는 DESC_FUNC 함수 포인터 하나만 가지고 있고, 이걸 실행함.
* mining() 함수에서 둘 중 하나가 랜덤으로 생성되어 minerals 벡터에 저장됨.
* edit_mineral_book()에서 UndiscoveredMineral을 고른다고 가정하고 description을 수정할 수 있음.
* 사용자는 UndiscoveredMineral로 가정하고 강제로 캐스팅해서 description에 문자열을 씀.
* 하지만 minerals[index]가 실제로는 RareEarth 객체일 수도 있음.
* RareEarth 객체는 DESC_FUNC description이라는 함수 포인터 하나만 가짐.
* 즉, RareEarth 객체를 UndiscoveredMineral*로 잘못 캐스팅하여 함수 포인터를 문자열로 오인하고 strncpy()로 덮어버림.

→ **그 결과 함수 포인터(RareEarth::description)를 우리가 원하는 주소로 임의로 조작 가능**



## 익스플로잇 시나리오

1. 희귀광물(RareEarth)을 먼저 획득
* mining()을 반복하여 RareEarth를 찾음 ("You found a rare-earth element!" 메시지가 뜸)
* 이때 minerals[i]에는 RareEarth*가 들어감

2. edit_mineral_book()으로 해당 i번 인덱스를 수정
* static_cast<UndiscoveredMineral*>로 잘못 캐스팅됨
* 입력한 값이 RareEarth의 함수 포인터(DESC_FUNC) 자리에 덮어짐

3. 그 후 show_mineral_book()으로 해당 인덱스를 출력
* RareEarth::print_description()이 실행되고 내부적으로 description()을 호출하므로 덮어쓴 주소가 실행됨

4. get_shell()의 주소를 덮어씌우면 쉘 획득 가능


  
