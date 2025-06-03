[Root Me] ELF x86 - Ptrace
=============================

ida로 바이너리를 보면

![image](https://github.com/user-attachments/assets/865f96ee-21b7-4c81-b5bb-2b66e72235f5)

ptrace 다음 jns를 통해 password를 입력받을지 프로그램을 바로 종료할지를 정하고 

![image](https://github.com/user-attachments/assets/1f97ad74-240a-42df-b2f3-65429e99a218)
![image](https://github.com/user-attachments/assets/bc3152d0-18df-4d0c-b672-51e7addef28a)

cmp al,dl 4번을 한 후에 Good password 인지 Wrong password 인지 출력한다.

![image](https://github.com/user-attachments/assets/a906f6e0-f5a3-470d-a9f2-ff27a85be5af)

먼저 ptrace 다음 jns 부분에 bp 설정해두고 실행 후 info r 하면 이렇게 나오는데

여기서 jns는 "Jump if Not Sign"의 약어이다. 즉 SF 플래그가 0일 때 (연산 결과가 음수가 아닐 때) 점프한다.

따라서 SF를 0으로 만들어 줘야 한다. 

![image](https://github.com/user-attachments/assets/1bbfc1a0-ecd1-40f9-8e67-d1b30e816ff7)
이거 참고해서 플래그 바꿔주면 


![image](https://github.com/user-attachments/assets/29aa2265-e085-4706-bb82-4aa5489efc1a)

이렇게 된다.

![image](https://github.com/user-attachments/assets/6f140fe1-958e-45fa-9763-1bf9bdd598a2)

ni 하다보면 password 입력할 수 있고


![image](https://github.com/user-attachments/assets/92b28aa7-b157-4841-bc43-036e58a6b0ef)

계속 가서 cmp al,dl 부분에 도착하면 dl 값을 al과 동일하게 맞춰준다.

이제 뒤에 나오는 세번의 cmp al,dl에서도 동일하게 반복해준다.

![image](https://github.com/user-attachments/assets/58709bcf-d85b-493a-b79c-9d50ac3db6f1)

![image](https://github.com/user-attachments/assets/d865d80a-1010-4359-9102-05e3c136afc7)

![image](https://github.com/user-attachments/assets/5fa6cb58-0231-47b4-b809-fba70d9ccb8b)


이렇게 하면 good password가 정상적으로 출력되는데 그러면 al에 들어있던 값들이 결국 password라 볼 수 있겠다.


0x65, 0x61, 0x73, 0x79 이므로 아스키 문자로 변환하면
* 0x65 (101 십진수) = e
* 0x61 (97 십진수) = a
* 0x73 (115 십진수) = s
* 0x79 (121 십진수) = y

![image](https://github.com/user-attachments/assets/27954317-3988-49b8-9c2a-0be3d3f7c5f8)
