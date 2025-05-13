evtx 파일 분석
================

### 1. Sysmon_13_Local_Admin_Password_Changed.evtx


![image](https://github.com/user-attachments/assets/d6d12d87-4d76-4f99-8aa9-3181e0924854)

* **Sysmon:** 이는 **System Monitor**의 약자로, 마이크로소프트에서 제공하는 Windows 시스템 활동 모니터링 도구이다.
  Sysmon은 프로세스 생성, 네트워크 연결, 파일 생성 시간 변경 등 다양한 시스템 이벤트에 대한 상세한 로그를 기록한다.
  따라서 이 파일 이름은 **Sysmon이 기록한 이벤트**와 관련이 있음을 나타낸다.


![image](https://github.com/user-attachments/assets/7fdc7c0e-bb61-43cf-b237-518b69fefb94)

* **13:** Sysmon은 다양한 이벤트 유형에 고유한 이벤트 ID를 할당한다.
  **이벤트 ID 13은 "Registry value set" 이벤트**를 나타낸다. 레지스트리 값이 변경될 때 발생하는 이벤트라고 한다.
  일반적으로 로컬 관리자 계정의 비밀번호 자체는 레지스트리에 직접 저장되지 않지만, 비밀번호 변경과 관련된 다른 레지스트리 키가 변경될 수 있다.
  따라서 파일 이름에 "13"이 포함된 것은 **레지스트리 값 변경 이벤트가 발생했으며, 이것이 로컬 관리자 비밀번호 변경과 연관되었을 가능성**을 보여준다.

* **Local\_Admin\_Password\_Changed:** 이 부분은 이벤트의 내용 즉 **시스템의 로컬 관리자 계정의 비밀번호가 변경되었다는 것**을 의미한다.


즉 파일 이름 "Sysmon\_13\_Local\_Admin\_Password\_Changed"에서는 다음과 같은 공격 시나리오를 예상할 수 있다.

1.   공격자가 시스템 내에서 권한을 획득한 후, 로컬 관리자 계정의 비밀번호를 변경하려고 시도한 경우: 이 과정에서 비밀번호 변경과 관련된 특정 레지스트리 값이 수정되었고,
  Sysmon 이벤트 ID 13이 기록되었을 수 있다.
2.   악성코드 또는 공격 도구가 로컬 관리자 비밀번호를 변경하는 과정에서 레지스트리 값을 조작한 경우: Sysmon은 이러한 레지스트리 변경을 이벤트 ID 13으로 탐지하고 기록했다.


![image](https://github.com/user-attachments/assets/a0c18d6b-c2d7-4d35-acfd-db91b179af03)

* **이벤트 ID 13 (레지스트리 값 설정):** 시스템에서 레지스트리 값이 변경되었다.
* **RuleName: Local Admin Password Setting Changed:** Sysmon 규칙에 의해 이름 붙여진 이벤트로, 로컬 관리자 비밀번호 설정 변경을 감지하기 위해 정의된 규칙임을 알 수 있다.
* **EventType: SetValue:** 레지스트리 값을 설정하는 동작이 발생했다.
* **UtcTime: 2020-09-20 21:22:24.798:** 이벤트가 발생한 UTC 시간이다.
* **ProcessGuid: {747f3d96-c6c1-5f67-0000-0010a65d0000}:** 레지스트리 값을 변경한 프로세스의 고유 식별자다.
* **ProcessId: 648:** 레지스트리 값을 변경한 프로세스의 ID이다.
* **Image: C:\Windows\system32\lsass.exe:** 레지스트리 값을 변경한 프로세스의 경로이다.
`lsass.exe`는 Local Security Authority Subsystem Service로, 로컬 보안 정책과 사용자 인증 등을 관리하는 시스템 프로세스이다.
* **TargetObject: HKLM\SAM\SAM\Domains\Account\Users\000001F4\ForcePasswordReset:** 변경된 레지스트리 키이다.
* **Details: Binary Data:** 변경된 레지스트리 값에 대한 추가적인 바이너리 데이터가 있다.
  이 데이터는 `ForcePasswordReset` 설정의 새로운 값(활성화 또는 비활성화)을 나타낸다.

결론적으로 이 Sysmon 이벤트 로그는 lsass.exe 프로세스가 로컬 관리자 계정의 비밀번호 재설정 강제 설정을 변경했다는 사실을 알려준다. 




### 2. sysmon_10_lsass_mimikatz_sekurlsa_logonpasswords.evtx 

![image](https://github.com/user-attachments/assets/7f6468e1-3595-4779-b065-5b85633e4232)

* **`10`**: 파일명에 숫자 `10`이 포함된 것으로 보아 Sysmon 이벤트 ID가 `10`번이라 예측할 수 있다.
  Sysmon 이벤트 ID 10은 **"Process Access" 이벤트**를 나타낸다.
 이는 하나의 프로세스가 다른 프로세스에 접근했을 때 기록되는 이벤트이다.
따라서 어떤 프로세스가 다른 프로세스에 접근하는 행위와 관련된 로그일 가능성이 높다.

(https://learn.microsoft.com/en-us/sysinternals/downloads/sysmon 참고) 

![image](https://github.com/user-attachments/assets/a14b42f9-7361-4861-8d4b-40180160cf8e)

* **`lsass`**: `lsass`라는 용어가 포함된 것으로 보아, 접근 대상 프로세스가 **LSASS (Local Security Authority Subsystem Service)** 이라 할 수 있다.
 LSASS는 윈도우 시스템에서 보안 정책, 사용자 인증 등을 관리하는 핵심 시스템 프로세스로, 계정 정보와 관련된 민감한 데이터를 메모리에 저장하고 있다.

* **`mimikatz`**: `mimikatz`라는 용어가 포함된 것으로 보아 접근을 시도한 프로세스 또는 관련 행위가
  **Mimikatz**라는 악성 도구와 관련이 있다고 보여진다.
   Mimikatz는 윈도우 시스템의 메모리에서 평문 비밀번호, Kerberos 티켓, NTLM 해시 등의 계정 정보를 추출하는 데 널리 사용되는 익스플로잇 도구이다.
  ![image](https://github.com/user-attachments/assets/02bdb94f-a55c-4af9-aeb4-54036b7e2798)


* **`sekurlsa`**: `sekurlsa`라는 용어는 Mimikatz 내의 특정 모듈 또는 기능과 관련이 있을 가능성이 높다.

* **`logonpasswords`**: `logonpasswords`라는 용어는 추출된 정보의 유형이 **로그온 비밀번호** 라고 보여준다.
* 혹은 `sekurlsa` 모듈 내에 `logonpasswords` 기능이 존재할 가능성도 높다. 실제로 구글링해보니 mimikatz 내에 sekurlsa::logonpasswords 기능이 존재한다고 한다.

![image](https://github.com/user-attachments/assets/a8a87737-4af6-4119-9736-d0df3b0e40a3)



즉 파일명 `sysmon_10_lsass_mimikatz_sekurlsa_logonpasswords.evtx`만으로 예측할 수 있는 공격 시나리오는 

**공격자가 Mimikatz라는 도구를 사용하여 윈도우 시스템의 LSASS 프로세스에 접근 (Sysmon 이벤트 ID 10)하여, `sekurlsa` 모듈의 `logonpasswords` 기능을 통해 
시스템에 로그인한 계정들의 비밀번호를 탈취하려고 시도했거나 성공했다.** 라고 볼 수 있다

이는 **계정 정보 탈취 공격** 을 명확하게 시사한다.


![image](https://github.com/user-attachments/assets/d43899ff-0cbe-4821-9693-1db7b9abf3f8)

이벤트 뷰어에서 이벤트 로그를 보면


* **UtcTime:** `2019-03-17 19:37:11.641`
    * 이 이벤트가 발생한 UTC (협정 세계시) 기준 시간이다. 2019년 3월 17일 19시 37분 11.641초에 이벤트가 기록되었음을 나타낸다.

* **SourceProcessGUID:** `{365abb72-a1e3-5c8e-0000-0010cef72200}`
    * 접근을 시도한 원본 프로세스의 고유 식별자 (GUID)이다. 이 GUID를 통해 시스템 내에서 해당 프로세스를 추적할 수 있다.

* **SourceProcessId:** `3588`
    * 접근을 시도한 원본 프로세스의 프로세스 ID (PID)이다. 이벤트 발생 당시 해당 프로세스의 PID를 나타낸다. 

* **SourceThreadId:** `2272`
    * 접근을 시도한 원본 프로세스 내의 특정 스레드 ID이다. 

* **SourceImage:** `C:\Users\IEUser\Desktop\mimikatz_trunk\Win32\mimikatz.exe`
    * 접근을 시도한 원본 프로세스의 실행 파일 경로이다. 명확하게 Mimikatz라는 악성 도구가 실행되었음을 보여준다.

* **TargetProcessGUID:** `{365abb72-0886-5c8f-0000-001030560000}`
    * 접근 대상 프로세스의 고유 식별자 (GUID)이다.

* **TargetProcessId:** `476`
    * 접근 대상 프로세스의 프로세스 ID (PID)이다.

* **TargetImage:** `C:\Windows\system32\lsass.exe`
    * 접근 대상 프로세스의 실행 파일 경로이다. 

* **GrantedAccess:** `0x1010`
    * 원본 프로세스가 대상 프로세스에 대해 획득한 접근 권한을 나타내는 16진수 값이다. `0x1010`은 `PROCESS_VM_READ` (메모리 읽기) 권한과 `PROCESS_QUERY_LIMITED_INFORMATION` (제한된 정보 쿼리) 권한의 조합으로, Mimikatz가 LSASS 프로세스의 메모리에서 계정 정보를 탈취하는 데 필요한 권한이다.

* **CallTrace:** `C:\Windows\SYSTEM32\ntdll.dll+4595c|C:\Windows\system32\KERNELBASE.dll+8185|C:\Users\IEUser\Desktop\mimikatz_trunk\Win32\mimikatz.exe+5c5a9|C:\Users\IEUser\Desktop\mimikatz_trunk\Win32\mimikatz.exe+5c86c|C:\Users\IEUser\Desktop\mimikatz_trunk\Win32\mimikatz.exe+5cbd2|C:\Users\IEUser\Desktop\mimikatz_trunk\Win32\mimikatz.exe+5c4ff|C:\Users\IEUser\Desktop\mimikatz_trunk\Win32\mimikatz.exe+3b3d3`
    * 이벤트 발생 시 호출 스택을 보여준다. Mimikatz 실행 파일 경로가 호출 스택에 포함되어 있어 Mimikatz가 직접적으로 LSASS 프로세스에 접근했음을 뒷받침한다. `ntdll.dll`과 `KERNELBASE.dll`은 윈도우 시스템의 핵심 DLL 파일이다.

요약하자면,

이 이벤트 로그는 `C:\Users\IEUser\Desktop\mimikatz_trunk\Win32\mimikatz.exe` (Mimikatz) 프로세스가 PID 3588을 사용하여 `C:\Windows\system32\lsass.exe` (LSASS) 프로세스 PID 476에 `0x1010` (메모리 읽기 등) 권한으로 접근한 것을 기록하고 있다. 이는 **Mimikatz를 이용한 계정 정보 탈취 시도**를 나타내는 증거라 볼 수 있다.
