my_pw.pcapng 라이트업
=============

![image](https://github.com/user-attachments/assets/7a99ef62-68fc-4b68-b7b0-b118fc964f65)

문제는 다음과 같다. 일단 주어진 파일을 와이어샤크로 열어본다. 특정 비밀번호를 알아내려면 웹과 관련된 프로토콜인 http를 봐야할 듯 하다. 

http로 필터링해서 패킷들을 하나씩 살펴본다. 그러면 join이나 login 이라는 문자열이 포함되어 있는 php페이지가 보이는 POST 패킷들이 보인다. 
이 패킷들을 하나씩 체크해서 비밀번호를 알아낼 수 있다.


![image](https://github.com/user-attachments/assets/b5d92b0b-bc9d-4b60-a92a-992f9a9f19ff)

첫번째 패킷이다. 그렇지만 id가 'zzanghacker'의 것으로는 보이지 않는다.


![image](https://github.com/user-attachments/assets/28b454fe-ae61-4e52-b789-24485358b90f)

join 문자열이 표시된 php페이지가 보이는 패킷 중 하나이다. zzanghacker가 가입 시 입력한 아이디, 비밀번호, 이메일이므로 이 패킷에서 보이는 IDISLIE 가 비밀번호라 볼 수 있다.
