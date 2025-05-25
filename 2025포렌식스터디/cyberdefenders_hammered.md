Cyberdefenders Hammered
==========================



![image](https://github.com/user-attachments/assets/33935f83-6e8e-482e-bd42-f5c4932e759f)

일단 문제 파일 안에는 이런 로그 파일들이 있다.

 Q1
 ---
![image](https://github.com/user-attachments/assets/dea86025-abfd-4937-bfc4-aeb242fda882)

첫 번째 문제는 공격자가 시스템에 접근하기 위해 사용한 서비스를 알아내는 것이다.

즉 공격자가 액세스 권한을 얻기 위해 어떤 수단을 사용했는지 알아내야 하므로 액세스 권한을 얻기 위한 시도에서의 인증 실패(authentication failures) 과정을 찾아봄으로써 알아낼 수 있을 것 같다.

auth.log를 찾아본다.

![image](https://github.com/user-attachments/assets/7ee31105-a0c9-44a5-bf9f-c39eaa57acad)

파일을 열어서 살펴보면 이렇게 액세스 권한이 accepted 로 표시된 부분들을 확인할 수 있다.

![image](https://github.com/user-attachments/assets/6e1d5da3-f5ea-4ad4-be30-7d398085ea9d)

따라서 답은 **ssh**



 Q2
 -----
![image](https://github.com/user-attachments/assets/7f480676-864b-4126-81ac-b26911c0639a)

파일들 중에는 dmesg 라는 파일이 있다.
![image](https://github.com/user-attachments/assets/cbb76c58-568b-4757-a673-0cb92a4b08c2)

 'dmesg'는 "display message"의 약어로, 부팅 과정 중에 커널이 출력하는 메시지를 보여주는 명령어의 출력 결과가 들어 있을 가능성이 높다.

 따라서 타겟 시스템의 운영 체제 버전을 알아내려면 dmesg 파일을 출력해보아야 한다.


![image](https://github.com/user-attachments/assets/8b2da6c4-4ce9-4f70-96b6-06ca261121f8)

head 명령어로 출력해보면 답은 **4.2.4-1ubuntu3**



Q3
--------
![image](https://github.com/user-attachments/assets/77932e6f-9496-4d06-887a-e5427a2757d1)


앞서 우리는 SSH에 대한 브루트포싱 공격 시도가 있다는 것을 알아냈다. 
유효한 사용자를 확인한 후에도 공격자는 비밀번호를 얻기 위해 브루트포싱을 계속할 것이다. 

따라서 비밀번호를 계속해서 여러번 틀린 특정 계정을 찾으면 된다.

auth.log 파일에서 grep "Failed password for" 이용해서 계정을 찾는다.

![image](https://github.com/user-attachments/assets/b24c10d7-d882-4fa7-84a7-cbf3fa7ffa07)

관찰해보면 이렇게 여러 invalid user 들이 이어지다가 어느 시점 후에는 쭉 root이 비밀번호를 얻으려 한다.

따라서 답은 **root**이라 할 수 있다. 



Q4
--------
![image](https://github.com/user-attachments/assets/b4fe1ed3-3f3e-4759-b0c4-2281a21fba18)

일단 cat auth.log | grep "Accepted password for root" | awk '{print $11}'  | sort -u  명령어를 사용해서root 계정으로 비밀번호를 사용하여 인증에 성공한 IP 주소들을 추출하고 중복을 제거하여 보여주도록 했다.


![image](https://github.com/user-attachments/assets/08940e48-db0e-4acc-8269-fc4c0b245242)

이제 이 ip들의 인증 실패 횟수를 비교해야 한다.
처음에는 grep "authentication failure" auth.log | awk '{print $(NF-3)}' | sort | uniq -c | sort -nr 명령어를 사용해봤는데 ip가 나오지 않는다. 

일단 grep "Accepted password for root" auth.log | awk '{for(i=1;i<=NF;i++) if ($i ~ /from/) print $(i+1)}' | sort | uniq > newlog 명령어 써서 비밀번호 입에 성공한 IP 주소들을 추출하고 중복을 제거하여 새 파일에 저장해뒀다. 

그리고 인증에 실패한 모든 ip들을 하나의 파일에 저장해두기 위해 grep "authentication failure" auth.log | awk '{for(i=1;i<=NF;i++) if ($i ~ /rhost=/ && $i ~ /[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+/) print $i}' | sed 's/rhost=//' | sort | uniq > new2log 명령어 사용해준다. 


![image](https://github.com/user-attachments/assets/2c5f2db6-b4be-4c09-9715-5fb8f1dec311)

이제 root로 로그인하는데에는 성공했지만 인증 실패를 겪은 ip 주소들을 새로운 파일에 저장해둔다. 
comm -12 newlog new2log > new3log 사용한다.

이제 new3log 파일에 있는 각 IP 주소에 대해 auth.log 파일에서 authentication failure 가 발생한 횟수를 세어야 한다. 지피티한테 물어봐서 하나의 명령어로 작성했다.

```bash
awk '
  BEGIN {
    while (getline < "new3log") {
      ips[$1] = 0 
    }
    close("new3log")
  }
  /authentication failure/ {
    match($0, /rhost=([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)/)
    ip = substr($0, RSTART + 6, RLENGTH - 6)

    if (ip in ips) {
      ips[ip]++
    }
  }
  END {
    for (ip in ips) {
      print ip ": " ips[ip]
    }
  }
' auth.log > auth_failure.txt 
```

명령어는 awk를 사용하며 BEGIN , 본문 규칙, END라는 세 가지 주요 블록으로 구성된다.

* **BEGIN 블록**:
    * awk가 실제 로그 파일을 읽기 **전에** 한 번 실행된다.
    * 여기서 **new3log 파일을 열어 그 안의 모든 IP 주소를 `ips`라는 연관 배열(associative array)에 저장** 한다. 각 IP 주소를 배열의 키로 사용하고 초기 실패 횟수 값은 0으로 설정한다. 이렇게 해서 `new3log`에 포함된 IP 주소들만 관심 대상임을 awk에게 알려주는 역할을 한다. 파일 읽기가 끝나면 `new3log` 파일은 닫힌다.

* **본문 규칙 (/authentication failure/ 블록)**:
    * 이 블록은 auth.log 파일의 각 줄을 처리하며, 해당 줄에 "**authentication failure**"라는 문자열이 포함되어 있을 때만 실행된다.
    * `match($0, /rhost=([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)/)`: 인증 실패 로그 줄(`$0`는 현재 처리 중인 줄 전체를 의미)에서 `rhost=` 뒤에 오는 **IP 주소 패턴을 찾아낸다.**
    * `ip = substr($0, RSTART + 6, RLENGTH - 6)`: `match` 함수를 통해 찾은 IP 주소 문자열만 깔끔하게 추출하여 `ip` 변수에 저장한다. (`rhost=` 6글자를 제외한 실제 IP 부분만 가져온다.)
    * `if (ip in ips) { ips[ip]++ }`: 추출된 ip 주소가 BEGIN 블록에서 new3log로부터 로드된 `ips` 배열에 **존재하는지 확인**한다. 만약 new3log에 있는 IP 주소라면 해당 IP 주소의 `ips` 배열 카운트를 1 증가시킨다.

* **`END` 블록**:
    * auth.log 파일의 처리가 **모두 끝난 후에** 한 번 실행된다.
    * `for (ip in ips) { print ip ": " ips[ip] }`: `ips` 배열에 저장된 모든 IP 주소와 그에 해당하는 최종 인증 실패 횟수를 "**IP 주소: 횟수**" 형식으로 출력한다.

* **`> auth_failure.txt`**: `awk` 명령어의 모든 출력 결과를 `auth_failure.txt` 파일로 저장한다.

이후에 cat 명령어 사용해서 auth_failure.txt의 내용을 출력한다.

![스크린샷 2025-05-25 211912](https://github.com/user-attachments/assets/8563d694-bbe1-4856-a67f-07a1b7520721)

1, 3같은 유의미하지 않은 값을 제외하면 답은 **6**개.






Q5
-------
![스크린샷 2025-05-25 211951](https://github.com/user-attachments/assets/c28d4023-e2fa-4800-ad48-b82f77d8b2de)

Q4에서 알아낸 6개의 ip 주소를 별도의 파일에 저장해둔다.

![스크린샷 2025-05-25 212155](https://github.com/user-attachments/assets/c8a7fd10-1010-4663-a896-f07a712a92b3)

이제 이 **attacker.txt 파일에 있는 각 IP 주소가 auth.log 파일에서 루트 계정으로 비밀번호를 통해 로그인에 성공한 횟수를 계산** 해야한다.

```bash
awk '
  BEGIN {
    while (getline < "attacker.txt") {
    }
    close("attacker.txt")
  }
  /Accepted password for root/ {
    for (i=1; i<=NF; i++) {
      if ($i == "from") {
        ip = $(i+1)
        if (ip in ips) {
          ips[ip]++
        }
        break
      }
    }
  }
  END {

    for (ip in ips) {
      print ip ": " ips[ip]
    }
  }
' auth.log
```


* **BEGIN 블록**:
    * awk가 로그 파일 처리(auth.log)를 시작하기 **전에** 한 번 실행된다.
    * `while (getline < "/tmp/ham/attacker_ip.txt")`: attacker.txt 파일을 열어 각 줄을 읽어온다. 각 줄은 IP 주소로 간주된다.
    * `ips[$1] = 0`: 읽어온 IP 주소(`$1`은 현재 줄의 첫 번째 필드)를 `ips`라는 연관 배열의 키로 사용하고, 해당 IP의 카운트 값을 0으로 초기화한다. 이렇게 하여 `attacker.txt`에 있는 IP 주소들만 aw`가 추적하도록 한다.
    * `close("attacker.txt")`: `attacker.txt` 파일과의 연결을 닫는다.

* **본문 규칙 (`/Accepted password for root/` 블록)**:
    * auth.log 파일의 각 줄을 읽으면서 해당 줄에 "**Accepted password for root**" 문자열이 포함되어 있으면 이 블록이 실행된다. 이는 루트 계정의 성공적인 비밀번호 로그인 메시지를 의미한다.
    * `for (i=1; i<=NF; i++) { if ($i == "from") { ip = $(i+1); ... break } }`: 현재 로그 줄(`$0`)의 각 필드를 반복하면서 "from"이라는 단어를 찾는다. "from"을 찾으면 그 다음 필드(`$(i+1)`)를 IP 주소로 간주하고 `ip` 변수에 할당한다.
    * `if (ip in ips) { ips[ip]++ }`: 추출된 ip 주소가 BEGIN 블록에서 로드된 `ips` 배열(즉 `attacker_ip.txt`에 있는 IP 목록)에 포함되어 있는지 확인한다. 만약 있다면, 해당 IP 주소의 `ips` 배열 카운트를 1 증가시킨다. `break`는 해당 줄에서 IP 주소를 찾았으니 더 이상 필드를 스캔하지 않도록 한다.

* **END 블록**:
    * auth.log 파일의 처리가 **모두 끝난 후에** 한 번 실행된다.
    * `for (ip in ips) { print ip ": " ips[ip] }`: `ips` 배열에 저장된 모든 IP 주소와 그에 해당하는 최종 루트 로그인 성공 횟수를 "**IP 주소: 횟수**" 형식으로 화면에 출력한다.


![image](https://github.com/user-attachments/assets/387a8ce8-b608-4eb6-b87e-dd4dcb3f2fc0)

실행하면 이렇게 출력 결과가 나온다. 답은 **219.150.161.20** 






Q6
----
![image](https://github.com/user-attachments/assets/3f31a93d-4890-44cf-9d6b-0345b4c04d76)

서버에 보내진 요청의 수를 알아내라는 질문인데 요청은 액세스와 밀접한 연관이 있다. (액세스 로그의 한 줄 = 요청)

따라서 apache 폴더의 www.access log를 이용한다. 

![image](https://github.com/user-attachments/assets/6798046f-1772-4277-9ea6-16f2587dc137)

wc -l www-access.log 명령어로 줄 수를 출력해보면 답은 **365**




Q7
------
![image](https://github.com/user-attachments/assets/20f224f0-49c2-4b2a-a10f-7fa8a3d9b498)

만들어진 방화벽 규칙 수를 묻고 있다.

![image](https://github.com/user-attachments/assets/bf746907-1693-45cc-9c2a-d36b0931799b)

찾아보니 리눅스 방화벽 규칙에 사용되는 대표적 툴이 **iptables** 라 한다.

**grep "iptables" auth.log** 명령어로 iptables 커맨드가 실행된 횟수를 알아본다.

![image](https://github.com/user-attachments/assets/b264d3b5-3505-43f3-a578-5470a9d7fef9)

root 에서 성공적으로 실행된 횟수는 **6** 번.



Q8
-----
![image](https://github.com/user-attachments/assets/5bd1bfdb-29c4-4c05-88ee-436c151c71c4)

스캐닝 툴 찾는 문제이다. 로그 파일 중 이런 데에 사용할 수 있는 파일은 찾아보니 **dpkg** 라 한다.

시스템에 설치되거나, 제거되거나, 업그레이드되거나, 구성된 모든 패키지의 변경 이력을 담고 있다.

따라서 dpkg.log 파일에서 install이나 configure 이라는 문자열을 찾으면 될 것 같다. 
다만 무조건 툴을 새로 인스톨했다고 가정할수도 없으니 configure을 사용하겠다.


![image](https://github.com/user-attachments/assets/df154a4a-f3dd-4e85-afbb-7e730dd593db)

**grep "configure" dpkg.log** 로 찾다 보면 **nmap** 이 보인다. (대표적 스캐너라 한다.)


Q9
----
![image](https://github.com/user-attachments/assets/84ce3945-d961-4158-b02b-b61078f727b4)

219.150.161.20 ip 가진 공격자가 마지막으로 로그인한 시각을 찾는 비교적 간단한 문제이다.

![image](https://github.com/user-attachments/assets/fdf63e9b-8b8c-4272-b467-a219e4bf6615)

**grep "Accepted password" auth.log | grep "219.150.161.20"** 써주면 된다. 

답은 **04/19/2010 05:56:05 AM**



Q10
-----
![image](https://github.com/user-attachments/assets/010e67a3-d79e-4a2c-b962-4ae596180c6f)

일단 데이터베이스의 warning에 대해 찾는 문제니까 **grep "mysql"** 로 모든 파일을 하나씩 살펴보면서 관련된 내용을 찾아본다. 

![image](https://github.com/user-attachments/assets/7559980e-d158-4425-99a6-fe8403a4bd47)

그러던 중 daemon.log에 유독 warning이 많은 것을 발견했다. **grep "warning" daemon.log** 으로 다시 찾아본다. 

![image](https://github.com/user-attachments/assets/dda5e317-c07a-456a-9984-71cebe7ad8bb)

대문자, 소문자로 한번씩 봐주었다. 

일단 주요 warning은 ' 1 client is using or hasn't closed the table properly' , ' mysql.user contains 2 root accounts without password!' , 'mysqlcheck has found corrupt tables' 이다.

이중에서 뭐가 제일 심각한 문제인지 추측해보면 역시 느낌표가 붙어있는 **mysql.user contains 2 root accounts without password!** 가 아닐까 추측했고, 확인 결과 정답이었다.






Q11
-------

![image](https://github.com/user-attachments/assets/e0a30c7e-e14e-45d4-95eb-59121ab07f75)


새로운 사용자 계정이 추가된 것을 보려면 'useradd' 문자열을 찾아야 한다.

**grep -i "useradd" auth.log** 명령어로 모든 사용자 계정 추가를 보고, Apr 26 04:43:15에 추가된 계정을
찾는다. -i는 10번 문제에서 대소문자 이슈로 헷갈렸던게 생각나서 붙여놨다. 


![image](https://github.com/user-attachments/assets/97788b50-6379-4650-94e0-77042c6818fc)


출력 결과를 보면 정답은 **wind3str0y**



Q12
----
![image](https://github.com/user-attachments/assets/64ad80de-d426-434b-9f4b-b099e84ed192)


일단, 공격자가 프록시를 사용했다면 해당 프록시 소프트웨어의 User-Agent가 로그에 기록될 가능성이 높다.
그러면 www-access.log 파일에서 User-Agent 필드를 추출하는 명령어 **awk -F'"' '{print $6}' www-access.log** 를 사용하여 문제를 해결할 수 있다.


이렇게 얻은 고유한 User-Agent 목록을 검토하여 일반적인 웹 브라우저나 검색 엔진 봇의 User-Agent와는 다른, 비정상적이거나 특정 프록시 소프트웨어의 특징을 나타내는 User-Agent 문자열을 식별할 수 있다.


![image](https://github.com/user-attachments/assets/464ed00e-0a80-4afe-ba43-18a32a1fc0b4)


출력 결과를 살펴보다 보면 apple, intel, mozilla 등 익숙한 이름들 사이에 **pxyscand/2.1**라는 낯선 이름이 보인다.

![image](https://github.com/user-attachments/assets/051ab885-8e8f-4797-a59f-ef43239ed385)

서치해보니 프록시 스캐너가 맞다. 따라서 정답은 **pxyscand/2.1**



![image](https://github.com/user-attachments/assets/69179408-f010-49ec-bf4e-2155d1ab23ed)

완료 
