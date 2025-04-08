Network Recoverly! 라이트업
=========================

![Network Recoverly! 문제](https://github.com/user-attachments/assets/ec999954-e61b-4e3b-9234-6eae3b98a763)

문제는 다음과 같다. 일단 플래그 구해서 그 값을 MD5 해시값으로 변환하면 되는 듯 하다.

파일을 와이어샤크에서 열어본다.

![image](https://github.com/user-attachments/assets/9e2f5cea-7570-4367-8675-b00970d55479)
중간중간 http 프로토콜로 get 요청을 받은게 눈에 띄인다. http 프로토콜로 필터링해본다.

![image](https://github.com/user-attachments/assets/ffb8dabe-c9b5-4bf5-9b35-dc5538681030)

![image](https://github.com/user-attachments/assets/dec1193c-5d41-451e-b554-d1e3e667f9ce)
필터링 결과 및 파일>객체 내보내기>http객체 목록을 확인해보면 treasure 1, 2, 3라는 각각 png, 텍스트, 텍스트 파일을 주고받은걸 볼 수 있다. 이 파일들의 헥스덤프를 확인해본다. 

![image](https://github.com/user-attachments/assets/16775fea-7d70-4e7c-b131-a1e40d730d8e)
treasure 1 에서는 일단 정상적으로 png 파일의 헤더 시그니처를 확인할 수 있다.

![image](https://github.com/user-attachments/assets/8cece474-36ed-4f3c-a721-cab98d7e861d)

treasure 2 에서는 유의미한 값을 관찰할 수 없었다.

그러나 treasure 3 에서는 파일 마지막에서 png 파일의 푸터 시그니처를 확인할 수 있었다.

즉 treasure 1, 2, 3은 하나의 png 파일을 세 부분으로 쪼개놓은 것이라 유추할 수 있다.

![image](https://github.com/user-attachments/assets/9e5f24f4-a30d-4db3-93fa-f7713f000b42)

헥스 에디터를 이용하여 세 파일의 헥사코드를 합쳐 새로운 파일을 만들고 내보낸다.

![image](https://github.com/user-attachments/assets/5901f8b8-b88b-4ee5-b095-4b4e3faae353)

확장자를 png 로 바꾸어 열어보면 이렇게 나온다. 이제 이 값을 MD5 해시값으로 바꾸어준다. 

MD5 해시값은 58504bd0fdcc2d955e69209b5dd4c59b이다. 따라서 형식에 맞는 플래그는 **lowercase(58504bd0fdcc2d955e69209b5dd4c59b)** 라 볼 수 있다. 


