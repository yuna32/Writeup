crack me 5
===============

![image](https://github.com/user-attachments/assets/3ded9b60-ed24-4e85-98f0-5958d8cb8436)

시리얼 번호를 입력하라고 하는 프로그램이다. 옳은 시리얼 번호를 찾아야 하는 것으로 보인다.

일단 아무런 값이나 넣은 다음에 디버깅을 시작했다.

내가 입력한 값으로 서치하면 이렇게 분기문 부분을 쉽게 찾을 수 있다.

![image](https://github.com/user-attachments/assets/0abd5805-cd4d-4ec5-865f-2cf2f621e7e4)

내가 작성한 문자열이 들어가 있는 004010BE ~ 004010C7 부분을 확인해보면

* PUSH crackme5.00402324는 문자열 "1234114"의 주소를 스택에 넣는다.
* PUSH crackme5.00402000은 "L2C-57816784-ABEX" 문자열의 주소를 스택에 넣는다.
* CALL <JMP.&KERNEL32.lstrcmpA>는 lstrcmpA 함수를 호출한다. 이 함수는 두 개의 문자열을 비교한다. 이 상황에서는 "L2C-57816784-ABEX"와 "1234114"를 비교하고 있다.

즉 여기서는 내가 입력한 값과 옳은 시리얼 넘버 **L2C-57816784-ABEX** 을 비교하고 있음을 추측할 수 있다.

![image](https://github.com/user-attachments/assets/041fb2a1-fa75-4ca5-9b41-53c55b3a4cda)

![image](https://github.com/user-attachments/assets/523517a5-c640-4c91-b469-486627de6f17)

이렇게 해결할 수 있다.
