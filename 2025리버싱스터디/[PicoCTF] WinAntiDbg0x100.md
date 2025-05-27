[PicoCTF] WinAntiDbg0x100
================

주어진 exe 파일을 실행하고 로그 창을 보면

![image](https://github.com/user-attachments/assets/2ab7f631-1a85-4e6f-a6da-14df7c7350a1)

디버거를 탐지하는 기능이 있고, 이를 해결해야 함을 알 수 있다.

![image](https://github.com/user-attachments/assets/7eea0a2e-8ec9-4da4-b2f9-9091ca572fd3)

심볼 정보 서치를 통해 IsDebuggerPresent라는 함수를 발견했다. 이 함수가 디버거를 탐지함을 짐작할 수 있다. 브레이크포인트 걸고 실행해본다.

![image](https://github.com/user-attachments/assets/ef408bcc-aec1-4caa-8504-045962135f3d)

디버거 탐지 문장이 출력되는 곳을 볼 수 있다. IsDebuggerPresent 함수 호출 후
프로그램은 디버거가 탐지되었는지 여부에 따라 조건부 점프 je를 수행한다. 이 조건부 점프는 디버거가 
탐지되면 디버거 탐지 문장을 출력한다.

![image](https://github.com/user-attachments/assets/65299b2e-40de-479c-9b42-49df3100f812)

그러면 je를 무조건 점프 명령어 jmp로 패치하여 해결할 수 있겠다.  

je (jump if equal) 명령어를 jmp (jump) 명령어로 변경하면
IsDebuggerPresent 함수의 반환 값에 관계없이 항상 디버거 탐지 문장을 뛰어넘고 플래그가 출력되는 부분까지 프로그램이 진행될 수 있게 한다.


이후 f9-f8 사용해서 프로그램 실행시킨 후에 로그창을 확인해본다.

![image](https://github.com/user-attachments/assets/359d4b6b-d308-4309-a6ae-69d38eee1076)






