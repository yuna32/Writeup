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






