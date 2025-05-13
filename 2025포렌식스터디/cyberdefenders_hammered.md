



![image](https://github.com/user-attachments/assets/33935f83-6e8e-482e-bd42-f5c4932e759f)

일단 문제 파일 안에는 이런 로그 파일들이 있다.

#### Q1
![image](https://github.com/user-attachments/assets/dea86025-abfd-4937-bfc4-aeb242fda882)

첫 번째 문제는 공격자가 시스템에 접근하기 위해 사용한 서비스를 알아내는 것이다.

즉 공격자가 액세스 권한을 얻기 위해 어떤 수단을 사용했는지 알아내야 하므로 액세스 권한을 얻기 위한 시도에서의 인증 실패(authentication failures)
과정을 찾아봄으로써 알아낼 수 있을 것 같다.

auth.log를 찾아본다.

![image](https://github.com/user-attachments/assets/7ee31105-a0c9-44a5-bf9f-c39eaa57acad)

파일을 열어서 살펴보면 이렇게 액세스 권한이 accepted 로 표시된 부분들을 확인할 수 있다.

![image](https://github.com/user-attachments/assets/6e1d5da3-f5ea-4ad4-be30-7d398085ea9d)


일단 제출한 후 재제출 기간에 마저 작성하겠습니다!!
