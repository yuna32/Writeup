crack me 3
===================

![image](https://github.com/user-attachments/assets/22bea122-2e89-487c-951e-5738d2568b97)

코드에 나타난 각 부분을 분석해보면



1.  **00401000 - 00401029:**
    * `PUSH` 명령어들을 통해 스택에 여러 값들을 넣고 있다.
    * PUSH crackme3.004020B9 명령어는 CreateFileA 함수의 첫 번째 인자로 전달될 파일 이름을 스택에 넣는 역할을 한다.
    * crackme3.004020B9는 메모리 주소를 나타내며
      004020B9 주소를 보면 CreateFileA 함수가 열거나  시도하는 파일 이름이 **abex.l2c**임을 알 수 있다.
    * `CreateWindowExA` 함수를 호출하여 MessageBoxA 클래스의 윈도우를 생성한다.
  

2.  **0040102A - 0040103C:**
    * `CALL DWORD PTR DS:[4020CA]`는 함수를 호출한다.
      `KERNEL32.CreateFileA` 함수를 가리키는 것으로 보아 파일을 여는 동작을 수행하는 것으로 보인다. `crackme3.004020B9`는 파일 경로를 나타내는 것 같다.
    * `CMP EBX, -1`과 `JE SHORT crackme3.00401075`는 파일 열기 결과(`EBX` 레지스터에 저장)를 확인하여 에러가 발생했는지 검사하고 에러 발생 시 00401075 으로 점프한다.

3.  **0040103D - 00401049:**
    * `CALL DWORD PTR DS:[4020C0]`는 또 다른 함수를 호출한다.
      `KERNEL32.GetFileSize` 함수를 호출하여 앞서 열린 파일의 크기를 얻어오는 것으로 보인다.
    * `CMP EAX, 12`와 `JNZ SHORT crackme3.00401060`는 파일 크기를 `12`와 비교하여 같지 않으면 00401060 으로 점프한다. 이는 필요한 파일의 크기가 12임을 보여준다.
     이게 몇 진수인지는 아직 알 수 없다.

4.  **0040104A - 0040105F:**
    * 파일 크기가 12 인 경우, "Well done!" 메시지 박스를 띄운다.

5.  **00401060 - 00401074:**
    * 파일 크기가 12 가 아닌 경우, "The found file is not a valid keyfile!" 메시지 박스를 띄운다.

6.  **00401075 - 00401088:**
    * `CreateFileA` 함수 호출이 실패한 경우, "Hmmm, I can't find the file!"  메시지 박스를 띄운다.

7.  **00401089 - 00401091:**
    * `CALL DWORD PTR DS:[4020B4]`는 `KERNEL32.ExitProcess` 함수를 호출하여 프로그램을 종료한다.



따라서 이 코드는 특정 크기(12)의 파일 abex.l2c가 이 프로그램과 같은 경로에 존재하는지 확인하고, 그 결과에 따라 다른 메시지를 사용자에게 보여준다.

![image](https://github.com/user-attachments/assets/5efb0da6-3cec-443b-9057-ba4db09e358f)

일단 12가 10진수라 가정하고 12바이트의 파일 abex.l2c를 같은 경로에 생성해준 후 check를 누른다. 그런데 옳지 않은 키파일이라 나온다.

즉 12는 10진수가 아니다. 그렇다면 12가 16진수 값이라 생각하고 이번엔 18바이트 파일을 만들어준다.

![image](https://github.com/user-attachments/assets/a160caa3-b501-4ee6-9f62-295925a9f363)

이렇게 해결된다.
