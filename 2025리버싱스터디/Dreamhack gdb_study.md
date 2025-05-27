Dreamhack gdb 학습
===============


버그와 디버거
---------------
### 버그(Bug)

프로그램 개발 시 아무리 신중하게 설계해도 코드의 규모가 커지면 실수가 발생하기 마련인데, 이러한 프로그램의 결함을 버그라고 한다.
컴퓨터 과학에서 공식적인 학술 용어는 아니지만 널리 쓰이는 용어이다.
어원은 1947년 하버드 대학의 Mark II 컴퓨터 회로에 나방이 들어가 오작동을 일으킨 것에서 유래했다.
현재는 프로그램에 의도치 않게 발생한 문제를 표현하는 데 사용된다.

### 디버거(Debugger)

개발자들이 버그를 잡는 데 많은 시간을 투자하며, 버그 없는 코드를 작성하기 어렵기 때문에 이러한 어려움을 해소하고자 디버거가 개발되었다.
문자 그대로 버그를 없애기 위해 사용되는 도구이다.
프로그램을 어셈블리 코드 단위로 실행하면서, 실행 결과와 프로그램의 내부 상황을 사용자에게 보여준다.
이를 통해 개발자는 추상적인 아이디어의 결과를 직관적으로 보며 자신이 작성한 코드의 문제점을 명확하게 찾을 수 있게 된다.
디버거는 해커, 리버스 엔지니어 등 소프트웨어에서 버그를 찾고 자하는 모든 이들이 버그 탐색의 효율을 높이는 데 사용된다.
버그 발견이 쉬워짐으로써 개발자들은 버그를 고치기 쉬워졌고, 해커들은 취약점을 발견하기 쉬워졌다.

gdb 디버거 실습
------------
![image](https://github.com/user-attachments/assets/94267a50-7428-4462-a6d0-f4de873a67d6)

![image](https://github.com/user-attachments/assets/e2865967-6cd2-4c44-9a2c-bfc98f895908)

gdb는 이미 설치되어 있으므로 드림핵에서 제공하는 파일로 실습을 따라가본다.

## 실행 흐름 제어 - run
![image](https://github.com/user-attachments/assets/7884c0c7-9be7-4c5d-973c-bb88dac94d40)

run 명령어는 프로그램을 실행하는 명령어이다. **run <프로그램 인자>** 와 같이 사용하며, 프로그램 인자가 필요 없는 경우 run 만 실행할 수 있다.


## 실행 흐름 제어 - break&continue


break는 특정 주소에 중단점-bp를 설정하는 기능이고, continue는 중단된 프로그램을 계속 실행시키는 기능이다.

중단점은 함수명과 주소를 이용해 걸 수 있다. break 대신 b 라는 단축키도 사용 가능하다.


![image](https://github.com/user-attachments/assets/b99be25c-a724-4ced-8a93-08b1c2692789)

첫번째는 **b main** 을 통해 main ㅎ마수에 중단점을 설정하고 있다. 해당 함수의 시작 주소인 0x401156에 중단점이 걸린다.

두 번째 경우는 **b *0x401156**을 통해 직접 중단점을 설정한다. 주소를 이용하는 경우 주소 앞에 *를 붙여야 한다.

continue 명령어를 사용하면 실행을 재개할 수 있다.

![image](https://github.com/user-attachments/assets/00808059-016f-4ce9-a4ae-16eab936a87c)


## 실행 흐름 제어 - entry&start

리눅스는 실행파일의 형식으로 elf를 규정하고 있으며, elf는 헤더와 여러 섹션들로 구성되어 있다. elf의 헤더 중에 진입점-ep 필드가 있는데, 
운영체제는 elf를 실행할 때 ep의 명령어부터 프로그램을 실행한다.

entry 명령어는 진입점부터 프로그램을 분석하게 해주는 명령어이다. 

![image](https://github.com/user-attachments/assets/ea3c6a8c-af7f-4fd7-8a1a-651d8dc15c3c)

entry 와 유사하지만 진입점이 아닌 main부터 분석할 수 있는 명령어로 start 명령어가 있다.
이를 수행하면 gdb에서 프로그램 실행 후 main의 위치를 찾아 멈춰준다. main을 찾을 수 없는 경우
entry와 동일하게 작동한다.


## 실행 흐름 추적

관찰하고자 하는 함수의 bp에 도달했으면 명령어를 한줄씩 분석해야 한다. 이때 **ni**와 **si** 명령어를 사용한다.


ni와 si는 모두 어셈블리 명령어를 한 줄 실행한다는 공통점이 있으나, ni는 함수의 내부로 들어가지 않지만
si는 함수의 내부로 들어간다는 차이점이 있다. (디버거에서의 단축키 f7-f8과 동일) 

![image](https://github.com/user-attachments/assets/38a0a7d5-4875-4f8c-899e-3cf0bba170d2)
![image](https://github.com/user-attachments/assets/00641dbb-c88c-4470-a7c3-d6ad2ec459ed)

main에서 printf를 호출하는 지점에 bp를 걸고 실행한다.

![image](https://github.com/user-attachments/assets/a0d56fa1-ef1f-4b95-a09c-124827111687)

ni를 입력하면 위와 같이 printf 바로 다음으로 rip 레지스터가 이동한 것을 확인할 수 있다.

![image](https://github.com/user-attachments/assets/be36284a-9e88-46bd-8800-53cea10d2a9e)

printf 호출 지점까지 다시 실행시킨 뒤, si를 입력하면 위와 같이 printf 내부로 rip가 이동한 것을 확인할 수 있다.

![image](https://github.com/user-attachments/assets/3aff0fda-cb8d-42fa-b70b-32c985f36c8e)


finish 명령어를 사용하면 함수의 끝까지 한번에 실행할 수 있다. si로 printf 내부에 진입한 상태에서 finish 명령어를 사용한 예시이다.



## 디버깅을 위해 자주 사용하는 명령어 - info


info 명령어는 정보를 보여주는 명령어이다.

![image](https://github.com/user-attachments/assets/6f8ed0ce-0d0c-4532-ade8-43f9a4ecfc01)

info register(i r로 단축키 사용 가능)은 레지스터에 저장된 값을 출력한다. 

이런 상황 외에 레지스터에 들어있는 값을 바로 사용하고 싶을 경우,

```
pwndbg> b $rdi
```

와 같이 $<레지스터명> 형식으로 바로 사용할 수 있다.

![image](https://github.com/user-attachments/assets/36b87c28-5165-4367-8719-3723f3a990b7)

info breakpoints(i b로 단축키 사용 가능)은 중단점들을 확인할 수 있는 명령어이다. 
각 중단점마다 번호가 부여된 것을 알 수 있고, 이 번호들을 이용하여 특정 중단점을 비활성화/활성화/삭제할 수 있다.

중단점을 비활성화하고 싶다면 disable 명령어, 활성화하고 싶다면 enable 명령어, 삭제하고 싶다면 delete(단축키 d) 명령어를 사용하면 된다.



## 디버깅을 위해 자주 사용하는 명령어 - disassemble

gdb는 기계어를 디스어셈블하는 기능을 기본적으로 탑재하고 있다. disassemble은 gdb가 기본적으로 제공하는 디스어셈블 명령어이다.

![image](https://github.com/user-attachments/assets/8c878b93-090c-4524-8258-379ca0e413c5)

위와 같이 함수 이름을 인자로 전달하면 해당 함수가 반환될때까지 전부 디스어셈블하여 보여준다.

![image](https://github.com/user-attachments/assets/1b13661a-aa1e-4294-be40-b551f80cfc1e)

u, nearpc, pdisass를 사용하면 디스어셈블된 코드가 가독성 좋게 출력된다.



## 디버깅을 위해 자주 사용하는 명령어 - x (examine)


x를 이용하면 특정 주소에서 원하는 길이만큼의 데이터를 원하는 방식으로 인코딩하여 볼 수 있다.

원하는 포맷과 크기를 조합하여 x와 결합한 명령어를 사용할 수 있다.

### rsp부터 80바이트를 8바이트씩 hex 형식으로 출력
![image](https://github.com/user-attachments/assets/4f1929a2-e697-44f4-9cc6-3b87d489c890)



### rip부터 5줄의 어셈블리어 출력
![image](https://github.com/user-attachments/assets/70a68925-0947-4566-a776-05110fa86442)


### 특정 주소의 문자열 출력
![image](https://github.com/user-attachments/assets/65aaeb6a-e6b1-4c97-901b-735580eb2734)


## 디버깅을 위해 자주 사용하는 명령어 - telescope

telescope(단축어 tele)는 pwndbg에서 제공하는 메모리 덤프 기능이다. 특정 주소의 메모리 값을 보여줄 뿐만 아니라 메모리가 참조하고 있는 주소를 재귀적으로 탐색하여 값을 보여준다.

![image](https://github.com/user-attachments/assets/2eb4d1be-097d-46c9-8608-592d1a1c69b9)


## 디버깅을 위해 자주 사용하는 명령어 - vmmap

vmmap은 가상 메모리의 레이아웃을 보여준다. 어떤 파일이 매핑된 영역일 경우(매핑이란, 어떤 파일을 메모리에 적재하는 것), 해당 파일의 경로까지 보여준다. 

![image](https://github.com/user-attachments/assets/d96179e2-38db-43b9-baf8-ae98c0d28994)



## 디버깅을 위해 자주 사용하는 명령어 - backtrace

콜 스택(call stack)이란 프로그램이 실행되는 동안 함수가 호출되는 순서를 저장하는 구조이다. 프로그램이 실행되면서 하나의 함수가 다른 함수를 호출할 때마다 새로운 함수 호출 정보가 추가되며, 함수 실행이 끝나면 제일 최근에 호출된 함수부터 순서대로 제거된다.

콜 스택은 디버깅 시 유용하게 사용할 수 있다. gdb에서는 backtrace(단축어 bt) 를 이용해 콜 스택을 확인할 수 있다.

![image](https://github.com/user-attachments/assets/81393d13-65ed-44d7-94b7-5b5d75467eed)

add를 호출한 뒤 콜 스택을 확인한 예시이다.



## 디버깅을 위해 자주 사용하는 명령어 - dump memory

프로세스의 메모리 상태를 파일로 저장할 때 사용하는 명령어로, **dump memory <파일명> <시작주소> <끝주소>** 와 같은 형식으로 사용한다.

![image](https://github.com/user-attachments/assets/7207ac3e-1ddb-464a-a7d1-ab47c3beb240)

프로그램 실행 후, vmmap 명령어로 프로그램의 코드 영역의 시작 주소와 끝 주소를 얻은 뒤 dump memory 명령으로 덤프를 수행한다.

![image](https://github.com/user-attachments/assets/99436d7b-52ed-408d-bfcc-232eb999c834)

이후 code_selection 이라는 파일이 생성된 것을 확인 가능하다.


## 디버깅을 위해 자주 사용하는 명령어 - context

pwndbg는 주요 메모리들의 상태를 프로그램이 실행되고 있는 맥락 즉 컨텍스트라고 한다. 
별도로 context 명령어를 입력하여 이 컨텍스트를 볼 수 도 있다.


context는 4개의 영역으로 구분된다.
1. registers: 레지스터의 상태를 보여준다.
2. disasm: rip부터 여러 디스어셈블된 결과를 보여준다.
3. stack: rsp부터 스택의 값들을 보여준다.
4. backtrace: 현재 rip에 도달할 때까지 어떤 함수들이 중첩되어 호출됐는지 보여준다.


## 디버깅을 위해 자주 사용하는 명령어 - set

set은 프로세스의 메모리 상태를 변경할 수 있는 명령어이다. 주로 레지스터 값을 변경하거나 특정 주소의 메모리 값을 변경하기 위해 사용한다. **set <주소/레지스터> = <변경할 값>** 와 같은 형식으로 사용할 수 있다.

![image](https://github.com/user-attachments/assets/7534f55a-8407-48c4-86ad-eb4458e04ac1)

레지스터 값 변경 예시

![image](https://github.com/user-attachments/assets/0ee879b8-26b0-44d4-a83b-ca460e96af6a)

특정 주소의 값을 바꾸기 위해서는 자료형이 필요하다. gdb는 자료형을 통해 해당 주소의 메모리 값을 어떻게 바꿀지 결정하고, c 언어의 자료형을 지원한다.

첫 번째 set 명령어 사용에서는 **메모리 주소 0x400000을 unsigned int*형으로 역참조한 후 정수 10을 저장** 한다. 따라서 0x400000부터 시작하는 4바이트 메모리 공간에 10을 기록하게 된다.

두 번째 set 명령어 사용에서는 **메모리 주소 0x400010을 float*형으로 역참조 한 후 부동소수점 값 3.14를 저장** 한다. 따라서 0x400010부터 시작하는 4바이트 메모리 공간에 3.14가 부동소수점 표현으로 저장된다.

이후 x(examine)으로 바뀐 메모리 값을 확인할 수 있다.














