crack me 2
============

![image](https://github.com/user-attachments/assets/1c28e943-100c-4316-b531-44ffff4fd5fc)

옳은 시리얼 넘버를 입력하는 프로그램인 듯 하다.

디버거를 켜고 search for > all intermodular calls 기능을 사용한다.

![image](https://github.com/user-attachments/assets/e77cdf34-513b-4bb8-9ea7-65aa0cbf0ca5)

함수들을 살펴보니 'tst'라는 말이 들어간 함수들이 자주 보인다. 일단 tsteq로 이동해보자.

![image](https://github.com/user-attachments/assets/fd718eab-aa54-48f7-bdf4-e95630e16aa0)

CALL DWORD PTR DS:[<&MSVBVM60.__vbaVarTstEq>] 부터 보자. 

VBA 런타임 라이브러리(MSVBVM60.dll) 내의 함수로, 이름 그대로 __vbaVarTstEq → “Variable Test Equal” 
즉 두 개의 변수를 비교하여 같으면 TRUE (AX=1), 아니면 FALSE (AX=0) 를 반환하는 함수이다.

이 부분의 코드를 분석해보면


* CALL DWORD PTR DS:[<&MSVBVM60.__vbaVarTstEq>]: 두 값을 비교하여 같으면 AX=1, 다르면 AX=0 반환
* TEST AX, AX / JE 00403941: AX 레지스터가 0이면 Zero Flag(ZF)=1 → JE (Jump if Equal),
  즉 두 값이 다르면 (아마도 값이 틀렸음을 알려주는 메시지박스 호출 함수로) 점프함
* MOV EBX, 800204 / MOV DWORD PTR SS:[EBP-4], EBX: EBX에 특정 값을 설정
* 이후 메시지 출력

즉 요약하면

1. 변수 두 개를 스택에 PUSH → (두 개의 비교 대상)
2. CALL __vbaVarTstEq → AX 레지스터에 결과 (0 또는 1)
3. TEST AX, AX → 결과가 0이면 ZF=1 (같지 않음)
4. JE → ZF=1일 때 점프 (두 값이 같지 않으면 점프)
5. 점프하지 않으면:
   - UNICODE "Congratulations!" 출력
   - UNICODE "Yep, this key is right!" 출력


여기서도 다른 crack me 문제들과 유사하게 옳은 시리얼 넘버가 정해져 있고, 사용자가 입력한 값을 그 시리얼 넘버와 비교하여 메시지박스가 출력된다고 추측할 수 있다.


![image](https://github.com/user-attachments/assets/68f716ed-cb26-4525-bfef-e1145523e154)

vbaVarTstEq 함수에 브레이크포인트 걸어주고 프로그램에 임의의 문자열을 입력하여 실행시킨다.

![image](https://github.com/user-attachments/assets/083dfd0f-687e-4177-8c10-651a46f783ad)

이후 스택을 확인해 보면 시리얼 넘버로 추정되는 값 **C5D7C8CA** 을 확인할 수 있다. 

![image](https://github.com/user-attachments/assets/72d32519-437a-410c-ac0b-9bc595dadadf)
