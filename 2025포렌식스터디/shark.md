shark.pcapng 라이트업
=====================


![image](https://github.com/user-attachments/assets/5d8db8b3-b45b-4d61-b149-523eb9996cae)

파일을 와이어샤크에서 연다. 13번째 패킷에서 secret.png라는 수상한 png 파일명이 보인다. 이 파일에 플래그가 숨겨져있을 가능성이 크다.


![image](https://github.com/user-attachments/assets/c8750294-042b-44e2-8bc9-43f0dc263245)
FTP -data로 필터링한다. FTP는 네트워크를 통해 파일을 전송하는데 사용되는 프로토콜이다. 여기서는 secret.png 파일을 업로드하고 있는 것으로 보인다. 

![image](https://github.com/user-attachments/assets/cfe22b66-4fc1-49ad-a8e9-8d1bd0f410af)

FTP-data 로 필터링하면 맨 위에 보이는 패킷의 헥사코드이다. png 파일의 헤더 시그니처가 보인다.

![image](https://github.com/user-attachments/assets/52d9fe96-7528-4c64-9658-d174a74226d8)

stor secret.png 중 맨 아래에 있는 패킷의 헥사코드이다. png 파일의 푸터 시그니처가 보인다.

![image](https://github.com/user-attachments/assets/834ec89b-99c4-49d1-b940-6bb4b6842d4a)

![image](https://github.com/user-attachments/assets/dbc5e8e9-a2e7-4959-b2b6-2314df759633)

따라가기 > tcp 스트림 해서 이렇게 raw 로 복사해서 헥스에디터에 붙여넣기 해주어 png파일 형태로 저장한다.

![무제1](https://github.com/user-attachments/assets/9689d611-5ba2-4966-93eb-d1d7b1ab4eb3)

그런데 이렇게 파일이 나온다. 시그니처도 다 멀쩡한데 추가로 다른 처리가 필요한걸까...

업로드된 다른 파일인 thumb-layer7도 추출해서 저장해봤지만 평범한 파일이다.

secret.png가 손상된거같은데 파일 카빙이 필요한걸까... 일단 파일을 foremost로 돌려봤는데 유의미한 결과는 안보인다. 
