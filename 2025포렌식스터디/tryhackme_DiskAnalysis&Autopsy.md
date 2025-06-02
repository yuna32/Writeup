tryhackme Disk Analysis & Autopsy
===================


1.What is the MD5 hash of the E01 image?
------------------------------------

![image](https://github.com/user-attachments/assets/56aa719f-65c9-4239-94c7-17124aa76a7b)

Data Source 클릭 후 메타데이터를 확인하면 알 수 있다.

**정답: 3f08c518adb3b5c1359849657a9b2079**



2.What is the computer account name?
-------------------------------------

![image](https://github.com/user-attachments/assets/e7b7836f-c7fa-4818-af50-855dc37b2ed2)

Results > Expanded Content > Operating System Information 순서로 열고 system에서 result를 확인한다.

**정답: DESKTOP-0R59DJ3**


3.List all the user accounts. (alphabetical order)
------------------------

![image](https://github.com/user-attachments/assets/1ad07140-317d-4868-b5af-a8691c380e30)

Operating System User Account 들어가서 User ID 클릭해서 정렬한다. 문제에서 요구하는 보통 user들은 date accessed 가 기록되어 있으므로 분류할 수 있다.

**정답: H4S4N,joshwa,keshav,sandhya,shreya, sivapriya,srini,suba**


4.Who was the last user to log into the computer?
-------------------------------------

![image](https://github.com/user-attachments/assets/be01df5e-e0d5-4005-8cba-e06ece34250f)

위의 문제와 같은 화면에서 Date Accessed 클릭해서 정렬한 후 맨 아래로 내린다.

**정답: sivapriya**


5.What was the IP address of the computer?
--------------------------------

![image](https://github.com/user-attachments/assets/aa65d4c1-fcd8-48cb-949d-20bf9d628ecf)

네트워크 모니터링 도구는 시스템의 네트워크 활동에 대한 기록을 남길 가능성이 높다. 이러한 도구는 IP 주소, 연결 기록, 전송량 등을 상세하게 로깅할 수 있다. 
Program Files에는 많은 애플리케이션이 설치되므로 네트워크 모니터링 도구가 여기에 설치되어 있을 가능성이 높다. 
네트워크 모니터링 도구의 설치 폴더 안에 해당 도구가 생성하거나 사용하는 설정 파일, 로그 파일, 데이터 파일 등이 있을 것이며, 이 파일 안에 시스템의 네트워크 정보가 포함될 수 있다.

따라서 Program Files(x86)으로 들어간다. 열어보면 look@lan이라는 낯선 이름이 보인다. 이것이 네트워크 모니터링 도구일 가능성이 높다. 안의 파일들의 내용을 살펴보던 중에 ip를 발견할 수 있었다.

**정답: 192.168.130.216**



6.What was the MAC address of the computer? (XX-XX-XX-XX-XX-XX)
--------------------------------------------------

![image](https://github.com/user-attachments/assets/4d4754d1-1014-4817-b22b-3112e34d44ef)


mac address의 형식을 찾아보니 XX-XX-XX-XX-XX-XX 형태로 숫자+알파벳 12개 조합인 듯하다.

그런데 위에서 찾은 ip 주소 아래에 숫자+알파벳 12개 조합이 있다. XX-XX-XX-XX-XX-XX로 기록되어 있지는 않지만 그 형태로 넣어보니 답이 맞았다.

**정답: 08–00–27–2c-c4-b9**



7.What is the name of the network card on this computer?
---------------------------------------------

![image](https://github.com/user-attachments/assets/56cf755d-4822-452d-9372-c8df6d5057e3)

network connection 찾기 위해서는 SOFTWARE 레지스트리 하이브로 들어가서 Microsoft\Windows NT\CurrentVersion 찾아야 한다. SOFTWARE의 application으로 들어가서 CurrentVersion을 찾아 NetworkCards 키를 찾으면 된다. 폴더들을 살펴보다보면 정답을 찾을 수 있다. 

**정답: Intel(R) PRO/1000 MT Desktop Adapter** 


8.What is the name of the network monitoring tool?
--------------------------------------------

![image](https://github.com/user-attachments/assets/0505b3a9-a7b3-4068-8671-f78b163b21f8)

위에서 찾은 look@lan이 정답일 것이므로 그냥 입력할 수도 있지만 Installed Program을 다시 확인해본다. 확실히 인스톨되어있는 것을 확인할 수 있다.

![image](https://github.com/user-attachments/assets/36c020ed-ccbc-4cd2-acf3-765d30362270)

구글링해보니 look@lan이 네트워크 모니터링 툴이 맞음을 확인할 수 있다. 

**정답: Look@LAN**



9.A user bookmarked a Google Maps location. What are the coordinates of the location?
----------------------------------------------------------

![image](https://github.com/user-attachments/assets/b1a99cb2-6142-48ce-bba0-6a3da0dd5488)

유저의 북마크를 보려면 Extracted Content에서 Web Bookmark를 찾는다. 여기서 위도경도를 찾으면 된다.

**정답: 12°52'23.0"N 80°13'25.0"E**


10.A user has his full name printed on his desktop wallpaper. What is the user’s full name?
-----------------------------------------------------------------------

![image](https://github.com/user-attachments/assets/906334a3-4f68-481f-9822-51c8e28a8f6f)

Images/Videos 탭에서 추출된 이미지와 비디오들을 볼 수 있다. 여기서 유저들의 폴더를 하나씩 찾아보면서 이미지를 찾아본다. joshwa 라는 유저가 Downloads 폴더에 이미지를 가지고 있는것을 확인할 수 있다.

![image](https://github.com/user-attachments/assets/4536d10a-598a-4d7e-8f82-b46de16961e9)

해당 이미지에 있는 글자를 잘 보면 'Anto Joshwa' 라는 풀네임을 볼 수 있다.

**정답: Anto Joshwa**


11.A user had a file on her desktop. It had a flag but she changed the flag using PowerShell. What was the first flag?
-----------------------------------------------------------------------------------------------

![image](https://github.com/user-attachments/assets/e83203d9-7877-4c8d-ac60-0e46937e4f0b)

users에서 각 유저들의 파워쉘 히스토리를 볼 수 있다. 

![image](https://github.com/user-attachments/assets/7096533b-366e-46c2-b9ed-0e4e5c92608d)

각 유저들의 파워쉘 히스토리를 찾아보다보면 Shreya라는 유저의 파워쉘 히스토리에서 I hacked you라는 플래그를 flag{HarleyQuinnForQueen} 으로 변경한 기록을 볼 수 있다.

**정답: flag{HarleyQuinnForQueen}**


12.The same user found an exploit to escalate privileges on the computer. What was the message to the device owner?
-------------------------------------------------------------------------------------------

![image](https://github.com/user-attachments/assets/96c37123-8c0d-45ec-a5d8-c30b379f8584)

위에서 찾아낸 유저 Shreya의 desktop 폴더로 가면 파워쉘 스크립트를 찾을 수 있다. 텍스트창을 열고 보면 I hacked you라는 메시지를 추가했다는 것을 알 수 있다.

**정답: Flag{I-hacked-you}**



13.2 hack tools focused on passwords were found in the system. What are the names of these tools? (alphabetical order)
-------------------------------------------------------------------


![image](https://github.com/user-attachments/assets/efb53ae5-3a0a-46ea-bae9-5c9e5ec6756d)


먼저 유저들의 다운로드 폴더를 찾아본다. 그 중 하나에서 mimikatz를 쉽게 찾을 수 있다.

![image](https://github.com/user-attachments/assets/c903625f-7fb9-43f1-a276-9345504079b5)

다른 하나는 result 트리의 Run Programs에서 찾아본다. 하나씩 살펴보다 보면 lazagne라는 이름의 수상한 프로그램이 보인다.


![image](https://github.com/user-attachments/assets/9cccb8cc-8308-45e8-af84-84c1437bed40)

구글링해보니 역시 문제에서 언급한 패스워드 관련 툴의 이름임을 알 수 있었다.

**정답: lazagne,mimikatz**


14.There is a YARA file on the computer. Inspect the file. What is the name of the author?
-----------------------------------------------------------------


![image](https://github.com/user-attachments/assets/f78595d4-3815-4731-8501-df915bf5cb1a)

keyword search로 yara 파일의 확장자인 .yar를 찾아본다. mimikatz가 위치해있는 폴더 안에 있음을 확인할 수 있다.


![image](https://github.com/user-attachments/assets/f04a3964-1fd5-431f-b7d5-7555baba27d2)

mimikatz_trunk.zip 안에서 해당 yar 파일을 찾아 author를 알아낼 수 있다.


**정답:Benjamin DELPY (gentilkiwi)**


15.One of the users wanted to exploit a domain controller with an MS-NRPC based exploit. What is the filename of the archive that you found?
------------------------------------------------------


![image](https://github.com/user-attachments/assets/4fe4111e-d6b7-4a91-aece-4332a6f686b2)

result의 Recent Documents에 가면 Zerologon이라는 수상한 이름의 lnk 파일이 보인다.

![image](https://github.com/user-attachments/assets/01a7b5af-fdf0-42b8-a240-7f2c12b55f54)

구글링해보니 문제에서 언급한 ms-nrpc 기반 익스플로잇과 연관된 취약점의 이름이라는 것을 알 수 있다.

**정답: 2.2.0 20200918 Zerologon encrypted.zip**





![image](https://github.com/user-attachments/assets/de1e5d43-ae4d-4fdb-9ad1-81ab374f1dd5)

완료


















