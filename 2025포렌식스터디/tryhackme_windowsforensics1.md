tryhackme windows forensics 1
===========================


task 2 문서화 및 문제풀이
-------------------


### **Windows 레지스트리 개요 및 구조**

* **Windows 레지스트리:** 시스템 구성 데이터(하드웨어, 소프트웨어, 사용자 정보)와
  최근 사용 파일, 프로그램, 연결된 장치에 대한 정보를 포함하는 데이터베이스이다. 
* **레지스트리 편집기(regedit.exe):** 레지스트리를 보고 편집하는 데 사용되는 Windows 유틸리티이다.
* **레지스트리 구성:** 레지스트리는 "키(Keys)"와 "값(Values)"으로 구성된다.
  "레지스트리 하이브(Registry Hive)"는 키, 서브키, 값을 단일 파일에 저장한 그룹이다.
* **루트 키:** 모든 Windows 시스템의 레지스트리에는 다음 다섯 가지 루트 키가 포함된다.
    1.  HKEY_CURRENT_USER
    2.  HKEY_USERS
    3.  HKEY_LOCAL_MACHINE
    4.  HKEY_CLASSES_ROOT
    5.  HKEY_CURRENT_CONFIG

### **레지스트리 편집기 실행 방법**

* **레지스트리 편집기 실행:** Windows 키와 R 키를 동시에 눌러 "실행" 프롬프트를 연다.
* **"실행" 프롬프트에 입력:** "regedit.exe"를 입력하면 레지스트리 편집기 창이 열린다.

### **루트 키 설명**

* **HKEY_CURRENT_USER:** 현재 로그인한 사용자의 구성 정보를 담고 있다.
  사용자의 폴더, 화면 색상, 제어판 설정 등이 저장된다. 약어로 HKCU로 불리기도 한다.
* **HKEY_USERS:** 컴퓨터에 활성화된 모든 사용자 프로필을 포함한다.
  HKEY_CURRENT_USER는 HKEY_USERS의 서브키이다. 약어로 HKU로 불리기도 한다.
* **HKEY_LOCAL_MACHINE:** 컴퓨터의 특정 구성 정보(모든 사용자용)를 담고 있다.
  약어로 HKLM으로 불리기도 한다.
    * `HKEY_LOCAL_MACHINE\Software\Classes`: 이 디렉토리에 저장된 정보는 Windows 탐색기에서
      파일을 열 때 올바른 프로그램이 실행되도록 한다. 약어로 HKCR로 불리기도 한다.
    * Windows 2000부터 이 정보는 HKEY_LOCAL_MACHINE과 HKEY_CURRENT_USER 키 아래에 저장된다.
    * `HKEY_LOCAL_MACHINE\Software\Classes` 키는 로컬 컴퓨터의 모든 사용자에게 적용되는 기본 설정을
      포함한다.
    * `HKEY_CURRENT_USER\Software\Classes` 키는 대화형 사용자에만 적용되는 설정을 포함한다.
* **HKEY_CLASSES_ROOT:** 레지스트리의 통합 보기를 제공하며 이 두 소스에서 정보를 병합한다.
  프로그램의 이전 버전용으로 만들어진 설정을 변경하는 데 사용될 수 있다.
  대화형 사용자의 설정을 변경하려면 `HKEY_CURRENT_USER\Software\Classes` 대신
  `HKEY_LOCAL_MACHINE\Software\Classes` 아래에서 변경해야 한다.
    * 기본 설정을 변경하려면 `HKEY_LOCAL_MACHINE\Software\Classes` 아래에서 변경해야 한다.
      이 키 아래에 값을 작성하면 시스템은 정보를 `HKEY_LOCAL_MACHINE\Software\Classes` 대신 이 키에
      저장한다.
    * 키 아래에 값을 작성하고 해당 키가 `HKEY_LOCAL_MACHINE\Software\Classes`에 이미 존재한다면,
      시스템은 정보를 `HKEY_CURRENT_USER\Software\Classes` 대신 `HKEY_LOCAL_MACHINE\Software\Classes`에
      저장한다.
* **HKEY_CURRENT_CONFIG:** 컴퓨터 시동 시 사용되는 하드웨어 프로필 정보를 담고 있다.

### 문제풀이
![image](https://github.com/user-attachments/assets/275a3a2d-7583-487b-8bb5-c8f8766b84e6)

HKEY_LOCAL_MACHINE의 약어는 **HKLM** 이다.




task 3 문서화 및 문제풀이
---------------------


### **레지스트리 하이브의 위치**

* **라이브 시스템 접근 시:** `regedit.exe`를 사용하여 레지스트리에 접근할 수 있으며,
  이전에 학습한 모든 루트 키를 볼 수 있다.
* **디스크 이미지 접근 시:** 레지스트리 하이브가 디스크에 어디에 위치하는지 알아야 한다.
  대부분의 하이브는 `C:\Windows\System32\Config` 디렉토리에 있다.
    1.  **DEFAULT** (HKEY_USERS\DEFAULT에 마운트됨)
    2.  **SAM** (HKEY_LOCAL_MACHINE\SAM에 마운트됨)
    3.  **SECURITY** (HKEY_LOCAL_MACHINE\Security에 마운트됨)
    4.  **SOFTWARE** (HKEY_LOCAL_MACHINE\Software에 마운트됨)
    5.  **SYSTEM** (HKEY_LOCAL_MACHINE\System에 마운트됨)
       
* **사용자 정보를 포함하는 하이브:**
    * 위 하이브 외에 사용자 정보를 포함하는 두 가지 다른 하이브가 사용자 프로필 디렉토리에서 발견될 수
      있다.
    * Windows 7 이상에서는 사용자 프로필 디렉토리가 `C:\Users\<username>`에 있으며, 두 하이브의 명칭은
        1.  **NTUSER.DAT** (사용자 로그인 시 HKEY_CURRENT_USER에 마운트됨)
        2.  **USRCLASS.DAT** (HKEY_CURRENT_USER\Software\CLASSES에 마운트됨)
     
    * USRCLASS.DAT 하이브는 `C:\Users\<username>\AppData\Local\Microsoft\Windows` 디렉토리에 있다.
    * NTUSER.DAT 하이브는 `C:\Users\<username>\` 디렉토리에 있다.
    * 이 파일들은 숨김 파일이다.

* **Amcache 하이브:**
    * 이 외에도 중요한 Amcache 하이브가 있다.
      이 하이브는 `C:\Windows\AppCompat\Programs\Amcache.hve`에 위치한다.
    * Windows는 시스템에서 최근에 실행된 프로그램에 대한 정보를 저장하기 위해 이 하이브를 생성한다.



### **트랜잭션 로그 및 백업**

* **트랜잭션 로그**
    * 포렌식 데이터의 중요한 소스이다. 레지스트리 하이브의 변경 로그로 간주될 수 있다.
    * Windows는 레지스트리 하이브에 데이터를 기록할 때 종종 트랜잭션 로그를 사용한다.
    따라서 레지스트리 하이브 자체에 아직 반영되지 않은 최신 변경 사항이 트랜잭션 로그에 포함될 수 있다.
    * 각 하이브의 트랜잭션 로그는 해당 하이브와 동일한 디렉토리에 `.LOG` 확장자를 가진 파일로 저장된다.
      예를 들어, SAM 하이브의 트랜잭션 로그는 `C:\Windows\System32\Config`에 `SAM.LOG`라는
      파일명으로 위치한다.
    * 때로는 여러 개의 트랜잭션 로그(`LOG1`, `LOG2` 등)가 있을 수 있다.
      레지스트리 포렌식을 수행할 때 트랜잭션 로그를 살펴보는 것이 중요하다.
      
* **레지스트리 백업**
    * 트랜잭션 로그와 반대되는 개념으로, `C:\Windows\System32\Config` 디렉토리에 위치한
      레지스트리 하이브의 백업이다.
    * 이 백업은 10일마다 `C:\Windows\System32\Config\RegBack` 디렉토리로 복사된다.
      최근에 레지스트리 키가 삭제/수정되었는지 의심되는 경우 확인하기에 좋은 장소이다.


### 문제풀이
![image](https://github.com/user-attachments/assets/e26ccedb-1919-493f-9a8e-52d8d10a9f91)

* 질문 1: 주요 레지스트리 하이브(DEFAULT, SAM, SECURITY, SOFTWARE, SYSTEM)의 경로는?
**C:\Windows\System32\Config**

* 질문 2: AmCache 하이브의 경로는?
**C:\Windows\AppCompat\Programs\Amcache.hve**
