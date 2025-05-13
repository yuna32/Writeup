crack me 4
=============

![image](https://github.com/user-attachments/assets/638d8944-2954-474f-80a4-8280f58e97b8)

올바른 시리얼 넘버를 찾아 registered 를 활성화해야 하는 문제로 보인다.

디버거에서 열고, 어떤 함수들이 사용되는지 보기 위해 search for > all intermodular calls 기능을 사용한다.


각 함수들의 기능을 찾아보며 살펴보면

![image](https://github.com/user-attachments/assets/931909b6-c4eb-421e-b5ce-1824d387d1a3)

vbaStrCmp 함수가 보인다. vbaStrCmp 함수는 두 문자열을 비교한다. 즉 내가 입력한 문자열과 정해져 있는 옳은 시리얼 넘버를 비교하는 것이다. 이 문제도 crackme5 문제와 비슷한 방법을 사용해 풀 수 있을 거 같다. 

![image](https://github.com/user-attachments/assets/6b3a59a7-ab6b-4e1e-b2a4-b0210d2d4755)

일단 vbaStrCmp 함수 위치로 이동해 브레이크포인트 걸어주고 프로그램을 실행시킨다.

문자열 입력하라는 메시지박스 뜨면 임의의 값을 입력해주고 이후 스택 확인하면

![image](https://github.com/user-attachments/assets/b9cc1cf3-a288-41bd-9a12-44045d91f1a5)

이렇게 브레이크포인트 건 부분에서 문자열 비교에 사용되는, 정해져 있는 시리얼 넘버 **2146500** 을 확인할 수 있다.

![image](https://github.com/user-attachments/assets/a8d2efa9-f618-4b80-a6bf-1a500a6bd679)

이렇게 해결할 수 있다.
