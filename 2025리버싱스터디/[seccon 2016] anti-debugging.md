[seccon 2016] anti-debugging
=====================


![image](https://github.com/user-attachments/assets/08aea401-d5e3-4ae2-a2ee-45bfdd28f09c)

주어진 exe 파일은 비밀번호를 요구한다. 문자열 참조를 통해 비밀번호가 "I have a pen." 이라는 것을 쉽게
알아낼 수 있지만 아래의 많은 에러 문장들을 보니 많은 안티디버깅 과정을 해결해야 플래그를 얻을 
수 있는 듯 하다.

![image](https://github.com/user-attachments/assets/ac6b3a39-10b9-4167-a1b1-7ad7cea45ce5)

ida를 켜서 main 함수를 살펴본다. 비밀번호가 맞더라도 IsDebuggerPresent 라는 함수를 call 하는게 보인다.

![image](https://github.com/user-attachments/assets/bee3f0e2-44b6-4270-8a33-5791d8291a31)

일단 아래로 내려가보면 암호화된 플래그로 추정되는 문자열이 보인다. 

![image](https://github.com/user-attachments/assets/635e3e15-5da3-4777-9245-8905da05e02c)

![image](https://github.com/user-attachments/assets/359f9bc7-a15d-4edd-9cd2-eb74a45aad12)

ida로 보면 이렇게 계속해서 분기되는 구조로 되어있음을 알 수 있다.

jnz, jz 문들을 모두 jmp로 바꾸어 패치해주고 (이때 jz는 뒤에 남는 바이트들 nop로 처리)


![image](https://github.com/user-attachments/assets/f23401cd-8e4d-457c-9077-4305f8db5638)


Divide by Zero 때문에 더이상 진행이 안된다고 뜨는 부분은  **코드 내에서 의도적으로 0으로 나누기(idiv) 연산을 수행하여 예외(exception)를 발생시켜 디버거를 탐지하는 부분** 이므로 해당 idiv 연산 부분에 set ip 설정해서 다음 코드 라인으로 명령어 포인터를 직접 이동시킨다.

그런데 이래도 계속 프로그램이 종료만 되어서 잘모르겠다.. 




