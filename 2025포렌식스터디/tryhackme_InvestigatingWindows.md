tryhackme Investigating Windows
==================

### 질문 1. Windows 시스템의 버전과 연도는?

![스크린샷 2025-05-20 201405](https://github.com/user-attachments/assets/f7368bf2-9854-49d5-8dde-6f9a7ed16669)


winver 명령어를 명령 프롬프트에 입력하면 현재 Windows 시스템의 버전 정보 등이 담긴 팝업창이 나타난다.

**Windows Server 2016**


### 질문 2. 마지막으로 로그인한 사용자는?

![스크린샷 2025-05-20 201505](https://github.com/user-attachments/assets/7338abbf-10ba-4454-87d3-965452fe5b93)

quser 명령어로 현재 시스템에 로그인한 사용자 목록을 출력한다.

마지막으로 로그인한 사용자 = 현재 시스템에 로그인한 사용자이므로 답은 **Administrator**.


### 질문 3. John은 시스템에 마지막으로 언제 로그인했나?

![스크린샷 2025-05-20 201932](https://github.com/user-attachments/assets/316927c4-b598-4a11-b8cc-7aefa86738f4)


`wevtutil query-events Security /format:text 
/q:"*[System[(EventID=4624)]] and *[EventData[Data[@Name='TargetUserName']='John']]" /c:1 /rd:true` 명령어를 사용한다. 


* **wevtutil query-events**:	Windows 이벤트 로그에서 이벤트를 쿼리하는 명령
* **Security**:	보안 로그(Security log)에서 이벤트를 찾는다.
* **/format:text**:	결과를 텍스트 형식으로 출력한다.
* **/q:"..."**: XPath 쿼리로 필터 조건을 지정한다.
* **/c:1**:	최대 1개의 이벤트만 출력한다.
* **/rd:true**:	최근 이벤트부터 역순으로 검색한다.


또 /q: 옵션 내부의 XPath 필터를 보면

```xpath
*[System[(EventID=4624)]] and *[EventData[Data[@Name='TargetUserName']='John']]
```

1. `EventID=4624`
   → **4624는 성공적인 로그인 이벤트**

2. `TargetUserName='John'`
   → 로그인한 대상 사용자 이름이 `'John'`인 경우만 필터링

즉, **"John이라는 사용자가 성공적으로 로그인한 최근 이벤트"** 를 1개 찾는 필터이다.

최종적으로 요약하면 **보안 로그에서 John이라는 사용자가 성공적으로 로그인한(이벤트 ID 4624) 
가장 최근 1개의 이벤트를 텍스트 형식으로 출력** 하는 명령어를 사용하여 John이 마지막으로 로그인한
시각을 구할 수 있다.

답은 **03/02/2019 5:48:32 PM**


### 질문 4. 시스템이 처음 시작할 때 연결하는 IP는?


![스크린샷 2025-05-20 211442](https://github.com/user-attachments/assets/873ff27c-793e-46f2-b4bf-b3b3921c3c12)

`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run` 레지스트리 경로를 확인하여 알아낸다.

이 레지스트리 키는 Windows 운영체제가 부팅된 후 자동으로 실행되도록 설정된 프로그램들의 목록을 저장한다. 
따라서 시스템 시작 시 실행되는 모든 프로그램이나 스크립트가 이 키에 등록될 수 있다.

![스크린샷 2025-05-20 211554](https://github.com/user-attachments/assets/568e8356-a64c-4d3a-837f-125c2790a98b)


답은 **10.34.2.3**


### 질문 5. Administrator 사용자 외에 관리자 권한을 가진 두 계정은?

![스크린샷 2025-05-20 202149](https://github.com/user-attachments/assets/33767b8d-6896-4e03-8618-f7cd17eb20df)

`net localgroup Administrators` 명령어로 로컬 "Administrators" 그룹의 모든 구성원을 나열한다.

답은 **Guest, Jenny**



### 질문 6. 악성 스케줄링 작업의 이름은?

시스템의 모든 스케줄링 작업을 나열하는 `schtasks /query /fo LIST /v` 명령어를 사용하여 알아내었다.

나열된 작업 중 Microsoft 기본 스케줄링 작업을 제외하면 다음과 같이 나온다.

![스크린샷 2025-05-20 202501](https://github.com/user-attachments/assets/54f27f59-6f12-4c90-b440-409ff3fe14e7)

![스크린샷 2025-05-20 202525](https://github.com/user-attachments/assets/2a0062e2-8a40-484e-a487-baa61a954c7c)

![스크린샷 2025-05-20 202534](https://github.com/user-attachments/assets/b471484b-3d48-4568-8268-7a33cfc4d1b4)

![스크린샷 2025-05-20 202541](https://github.com/user-attachments/assets/ae0885a5-d829-451c-bc79-aa5c8b77e263)

이 중 답안의 빈칸에 맞는 작업은 **Clean file system** 이다.


### 질문 7. 해당 작업이 매일 실행하려고 했던 파일은?


위 출력 결과에서 Clean file system 작업의 항목을 자세히 보면

![스크린샷 2025-05-20 202723](https://github.com/user-attachments/assets/e4d61684-266b-45fb-a7e5-007eb633407e)

task to run 항목에서 **C:\TMP:\nc.ps1** 이 보인다. 즉 **nc.ps1** 라는 파일을 매일 실행하려 했던 것으로 보인다.


### 질문 8. 이 파일이 로컬에서 수신 대기했던 포트는?

![스크린샷 2025-05-20 202723](https://github.com/user-attachments/assets/7b72ab79-bde0-4fdb-90bf-37bcceee8ed0)


이것도 질문 6의 출력 결과의 task to run 항목 맨 뒤에 붙어있는 **1348** 이 포트 번호이다.


### 질문 9. Jenny는 마지막으로 언제 로그인했나?

질문 3과 동일한 명령어를 사용했다.

![스크린샷 2025-05-20 203930](https://github.com/user-attachments/assets/1f611a96-c375-4f51-8725-5e268470a9d8)

그런데 아무것도 출력되지 않는다.

다시 생각해보면, 질문 3과 동일하게 로그인한 시각을 묻는데 답변 형식을 월/일/년/시각으로 주지 않은게 이상하다.

즉 Jenny는 아예 로그인한 적이 없다는 가정을 해볼 수 있다.

답변의 빈칸이 5개이므로 **Never**을 입력해야 정답이다.


### 질문 10. 침해 사고는 어떤 날짜에 발생했나?

![스크린샷 2025-05-20 204501](https://github.com/user-attachments/assets/e4242d88-40b9-490d-b6bf-7841251d0c86)


아까 악성 스케줄링 작업으로 판명된 Clean file system의 start date를 체크해보면 이 질문의 답을 알 수 있다.

답은 **03/02/2019**


### 질문 11. 침해 사고 중 Windows가 새 로그온에 특별한 권한을 처음 부여한 시각은?

이벤트 뷰어를 통해 알아보자.

![스크린샷 2025-05-20 210359](https://github.com/user-attachments/assets/fb4f22b7-80dc-4782-85b9-fca2aaed11b4)

위에서 알아냈다시피, 침해 사고가 일어난 일자는 03/02/2019 이다. 그렇다면 해당 날짜로 필터링을 한다.
또 위에서 알아낸 내용들을 보면 Clean file system과 같은 악성 스케줄링 작업이 4시경에 일어났음을 알 수 있다.
따라서 4시~5시로 시각도 필터링한다.

![스크린샷 2025-05-20 210428](https://github.com/user-attachments/assets/ce936066-4c9a-4c6c-afd6-c421be392871)

logs 설정에서 security를 선택해주고 확인한다.

출력된 결과에서 일단 문자열 'security'가 처음 보인 항목은 이것이다.


![스크린샷 2025-05-20 211226](https://github.com/user-attachments/assets/f3cf4f9d-0730-4776-a7e8-d58209987d04)
하지만 자세히 내용을 확인해보면 단순히 시스템 시각이 바뀐 것이므로 이건 아닐거 같다.

다음 항목은 이것이다. 


![스크린샷 2025-05-20 211149](https://github.com/user-attachments/assets/0bf54e1a-c71d-4298-870d-b217e804f177)

security-enabled local group이 change 되었다고 한다. 일단 이게 그럴듯해 보여서 입력해보니 정답이라 한다.


답은 **03/02/2019 4:04:47 PM**




### 질문 12. Windows 암호를 얻는 데 사용된 도구는?

명령 프롬프트로 문제들을 풀다 보면 자꾸 C:\TMP\mim.exe 창이 뜨는 것을 알 수 있다. 
명령 프롬프트의 역할을 생각해보면 해당 실행 프로그램이 매우 수상하다고 생각된다.

type 명령어로 해당 프로그램을 텍스트로 띄워보자.

![스크린샷 2025-05-20 205027](https://github.com/user-attachments/assets/de40b37a-e7f0-4b3b-917a-cbb0dcf2fe92)

그러면 바로 이렇게 mimikatz 가 보인다. 즉 사용된 도구는 **mimikatz** 라 할 수 있다.


### 질문 13. 서버 웹사이트를 통해 업로드된 셸의 확장자 이름은?

![스크린샷 2025-05-20 205500](https://github.com/user-attachments/assets/fd2535ee-03ab-4706-8ef1-0c55a103b57f)

`dir C:\inetpub\wwwroot /tc` 명령어를 사용한다. 디렉터리의 파일 및 폴더 목록을 보여주는 dir 명령어에
파일의 "생성 시간(Creation Time)"을 기준으로 표시하는 /tc 옵션을 붙인 것이다. 또 C:\inetpub\wwwroot 디렉터리
(일반적으로 IIS 웹 서버의 기본 웹사이트 루트) 내의 파일 및 폴더 목록을 가져와 서버 웹사이트를 통해 업로드된 파일들을 출력한다.


출력된 결과를 보면 주요 확장자는 .jsp 혹은 .gif 둘 중 하나이다. 두 개를 모두 입력해본 결과 정답은 **.jsp**


### 질문 14. 공격자가 마지막으로 연 포트는?

방화벽 로그를 통해 기록된 허용되거나 차단된 연결 시도를 확인하여 공격자가 연 포트를 알 수 있다.

![스크린샷 2025-05-20 205937](https://github.com/user-attachments/assets/ece2cc4e-4cba-4056-9452-e6f0384e8b4f)

Windows Firewall 켜고 inbound rules 클릭해주면 이렇게 시간별로 쭉 뜬다. 

정답은 맨 위에 있는 포트 번호인 **1337**


### 질문 15.  DNS 변조를 확인하라. 어떤 사이트가 표적이 되었나?

DNS 변조 여부를 확인하는 데 중요한 파일인, 도메인 이름을 IP 주소에 수동으로 매핑하는데 사용하는 hosts 파일의 내용을 화면에 출력해야 한다.
사용되는 명령어는 `type C:\Windows\System32\drivers\etc\hosts` 이다. 

![스크린샷 2025-05-20 205343](https://github.com/user-attachments/assets/4760cea2-dd6f-444b-b5fd-a15cef6fdf34)

![스크린샷 2025-05-20 205353](https://github.com/user-attachments/assets/6b78994d-7cd2-4a3f-92f4-6c42e05b2b97)

출력된 결과를 보면 microsoft update를 제외하고 거의 모든 도메인들이 127.0.0.1 ip주소에 매핑되어 있는데
google.com만 다른 ip가 사용되고 있다.


따라서 정답은 **google.com** 

