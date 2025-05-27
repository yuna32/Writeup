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

그러면 이 분기들을 jmp를 아마 쓰면 될거같긴한데 어떻게 써야할질 모르겠다... 
