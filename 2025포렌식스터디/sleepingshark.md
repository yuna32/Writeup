sleepingshark 라이트업
====================





![image](https://github.com/user-attachments/assets/6d569464-0155-44fd-8439-26dbdb1f3344)
와이어샤크로 파일을 열어보면 수상해보이는 post 요청이 있다.


![image](https://github.com/user-attachments/assets/650517a8-259e-412a-99a4-480cd82466e1)
여러 디코딩을 시도해보다가 url 디코딩을 해보니 이렇게 나왔다. 형태를 보니 sql 쿼리문이다. 쿼리를 분석해보자.

IF() 함수를 사용하여 조건부 동작을 수행한다. IF(condition, value_if_true, value_if_false) 형식이다.

- (SELECT flag FROM s3cr3t LIMIT 1): s3cr3t 테이블에서 flag 컬럼의 첫 번째 행(LIMIT 1)을 선택하는 서브쿼리이다.

- SUBSTRING(..., 35, 1): 위에서 선택된 flag 값의 35번째 문자부터 1개의 문자(즉, 35번째 문자)를 추출한다.

- ASCII(...)= 156: 추출된 문자의 ASCII 코드 값을 반환하여 해당 값이 156과 같은지 비교한다.

- SLEEP(3): 조건이 참(ASCII 코드 값이 156)이면 3초 동안 멈춘다.

- 0: 조건이 거짓(ASCII 코드 값이 156이 아님)이면 0을 반환한다.

즉, 이 쿼리는 s3cr3t 테이블의 flag 컬럼의 첫 번째 값의 35번째 문자의 ASCII 코드 값이 156인지 확인한다. 만약 그렇다면 3초 동안 멈추고, 그렇지 않다면 0을 반환한다.


이러한 형태의 쿼리는 Time-based SQL Injection에 사용된다고 볼 수 있다.

여기서 우리는 참/거짓 여부에 따른 시간 지연을 통해 플래그에 다가갈 수 있다. 

즉 시간이 3초보다 오래 걸린 패킷들을 골라내서 그 아스키 코드 값들을 모으면 플래그가 나온다고 생각했다.

![image](https://github.com/user-attachments/assets/00d483f5-a9c4-4296-821b-ccfbd84f7852)

먼저 http를 필터링 해준다. 그런데 양이 생각보다 많아서 코드를 작성해서 해결해야할 것 같다. 

노가다하자면 끝이 없을거같아서... 


pcap 파일 관련은 아직 잘 몰라서 지피티 참고해서 파이썬 코드 작성했다.

```python
from pwn import * 
import dpkt
from dpkt.compat import compat_ord
import urllib


def analyze_packets(pcap):
    flag = ['X'] * 39
    post_sent = False
    payload = ''
    request_time = 0
    response_time = 0

    for timestamp, buf in pcap:
        eth = dpkt.ethernet.Ethernet(buf)
        if not isinstance(eth.data, dpkt.ip.IP):
            continue

        ip = eth.data
        tcp = ip.data

        if not post_sent and tcp.dport == 80 and len(tcp.data) > 0:
            try:
                http = dpkt.http.Request(tcp.data)
                if http.method == 'POST':
                    request_time = timestamp
                    payload = urllib.parse.unquote(http.uri)
                    post_sent = True
            except (dpkt.dpkt.NeedData, dpkt.dpkt.UnpackError):
                continue

        elif post_sent and tcp.sport == 80 and len(tcp.data) > 0:
            response_time = timestamp
            try:
                dpkt.http.Response(tcp.data)
                if response_time - request_time >= 2.8:
                    payload = payload[payload.find('LIMIT 1),') + 9:]
                    idx = int(payload[:payload.find(',')]) - 1
                    ch = chr(int(payload[payload.find('))=') + 3:payload.find(', SLEEP(3)')]))
                    flag[idx] = ch
            except (dpkt.dpkt.NeedData, dpkt.dpkt.UnpackError, ValueError, IndexError):
                continue

            post_sent = False

    return ''.join(flag)


def main():
    with open('C:\\Users\\gram\\Downloads\\dump.pcap', 'rb') as f:
        pcap = dpkt.pcap.Reader(f)
        flag = analyze_packets(pcap)
        print(flag)


if __name__ == '__main__':
    main()
```

코드의 주요 동작을 간단히 요약하면 다음과 같다.


1.  **패킷 파일 읽기:** `main()` 함수에서 지정된 경로의 패킷 캡처 파일을 읽어온다
2.  **패킷 분석 함수 (`analyze_packets`):**
    - 읽어온 패킷 데이터를 순회하며 분석한다.
    - **HTTP POST 요청 감지:** 대상 포트가 80번이고 데이터가 있는 TCP 패킷 중 HTTP POST 요청을 찾는다. 요청 시간을 기록하고 요청 URI에서 sql 쿼리문이 담긴 페이로드를 추출한다. 
    - **HTTP 응답 감지:** POST 요청 이후에 출발지 포트가 80번이고 데이터가 있는 TCP 패킷을 HTTP 응답으로 간주하여 응답 시간을 기록한다.
    - **시간 지연 기반 플래그 추측:** 요청 시간과 응답 시간의 차이가 2.8초 이상이면 (sql 쿼리가 `SLEEP(3)` 실행하므로), 페이로드에서 플래그의 특정 문자와 인덱스 정보를 추출한다. 페이로드에서 `LIMIT 1),` 이후의 부분을 파싱하여 플래그의 인덱스와 해당 위치의 ASCII 코드 값을 얻어내고 이를 문자로 변환하여 `flag` 리스트에 저장한다.
    - **오류 처리:** HTTP 요청 및 응답 파싱 과정에서 발생할 수 있는 `dpkt.dpkt.NeedData`, `dpkt.dpkt.UnpackError`, `ValueError`, `IndexError` 등의 오류를 `try-except` 구문으로 처리하여 프로그램이 중단되지 않도록 한다.
3.  **플래그 재구성 및 출력:** `analyze_packets` 함수에서 추출된 플래그 문자들을 리스트에 저장하고 최종적으로 이 리스트의 문자들을 결합하여 하나의 문자열 형태의 플래그를 반환한다. `main()` 함수에서는 이 플래그를 화면에 출력한다.


![image](https://github.com/user-attachments/assets/0b4deb16-188f-4cef-8610-2d5c11abd1f9)

이렇게 플래그를 얻어낼 수 있다.
