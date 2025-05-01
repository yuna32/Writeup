shark.pcapng 라이트업
=====================


![image](https://github.com/user-attachments/assets/5d8db8b3-b45b-4d61-b149-523eb9996cae)

파일을 와이어샤크에서 연다. 13번째 패킷에서 secret.png라는 수상한 png 파일명이 보인다. 이 파일에 플래그가 숨겨져있을 가능성이 크다.


![image](https://github.com/user-attachments/assets/c8750294-042b-44e2-8bc9-43f0dc263245)
FTP -data로 필터링한다. FTP는 네트워크를 통해 파일을 전송하는데 사용되는 프로토콜이다. 여기서는 secret.png 파일을 업로드하고 있는 것으로 보인다. 

![image](https://github.com/user-attachments/assets/cfe22b66-4fc1-49ad-a8e9-8d1bd0f410af)

FTP-data 로 필터링하면 맨 위에 보이는 패킷의 헥사코드이다. png 파일의 헤더 시그니처가 보인다.

![image](https://github.com/user-attachments/assets/52d9fe96-7528-4c64-9658-d174a74226d8)

stor secret.png 중 맨 아래에 있는 패킷의 헥사코드이다. png 파일의 푸터 시그니처가 보인다.

![image](https://github.com/user-attachments/assets/834ec89b-99c4-49d1-b940-6bb4b6842d4a)

![image](https://github.com/user-attachments/assets/dbc5e8e9-a2e7-4959-b2b6-2314df759633)

따라가기 > tcp 스트림 해서 이렇게 raw 로 복사해서 헥스에디터에 붙여넣기 해주어 png파일 형태로 저장한다.

![무제1](https://github.com/user-attachments/assets/9689d611-5ba2-4966-93eb-d1d7b1ab4eb3)

그런데 이렇게 파일이 나온다. 시그니처도 다 멀쩡한데 추가로 다른 처리가 필요한걸까

업로드된 다른 파일인 thumb-layer7도 추출해서 저장해봤지만 평범한 파일이다.

+재제출 추가

Identification 필드란 하나의 원래 IP 패킷이 여러 조각(fragment)으로 분할(fragmentation) 되었을 때, 모든 조각이 같은 원래 패킷에 속한다는 것을 식별하기 위해 사용된다.

즉 같은 Identification 값을 가진 조각들은 하나의 원래 IP 패킷에 속한다. 

![image](https://github.com/user-attachments/assets/b612227e-b823-494c-a0dc-1cb40ee081d8)

STOR Secret.png 문자열이 info에 포함된 패킷들을 보면 공통적으로 0x0800이라는 Identification 값을 가지고 있다. 

Secret.png 파일을 복구하려면 결국 STOR Secret.png 문자열이 info에 포함된 패킷들을 
Fragment Offset 순서대로 배열해야 하는데 위에서 수동으로 한 방법이 통하지 않았으므로 이번에는
코드를 작성해서 해결해보기로 했다. 

방법들이 여러 가지 있어서 지피티한테 물어보면서 작성했다.

```python
from scapy.all import *
import os
import binascii
import re

def extract_png_from_pcap(pcap_file):
    print(f"Opening PCAP file: {pcap_file}")
    
    # 패킷 로드
    try:
        packets = rdpcap(pcap_file)
        print(f"Successfully loaded {len(packets)} packets")
    except Exception as e:
        print(f"Error loading pcap file: {e}")
        return
    
    # PNG 시그니처와 종료 마커
    png_signature = b'\x89PNG\r\n\x1a\n'
    png_end_marker = b'IEND\xaeB`\x82'
    
    # 모든 IP 패킷의 ID를 기준으로 그룹화
    fragments_by_id = {}
    
    # 첫번째 방법: IP 조각들을 ID별로 그룹화
    for packet in packets:
        if IP in packet:
            ip_id = packet[IP].id
            if ip_id not in fragments_by_id:
                fragments_by_id[ip_id] = []
            fragments_by_id[ip_id].append(packet)
    
    # 각 ID 그룹별로 처리
    for ip_id, fragments in fragments_by_id.items():
        # Fragment Offset으로 정렬
        sorted_fragments = sorted(fragments, key=lambda p: p[IP].frag if hasattr(p[IP], 'frag') else 0)
        
        # 패킷 데이터 추출 및 결합
        reassembled_data = b''
        for fragment in sorted_fragments:
            # IP 패킷의 페이로드 추출
            if Raw in fragment:
                reassembled_data += fragment[Raw].load
            elif TCP in fragment and hasattr(fragment[TCP], 'payload'):
                reassembled_data += bytes(fragment[TCP].payload)
            elif UDP in fragment and hasattr(fragment[UDP], 'payload'):
                reassembled_data += bytes(fragment[UDP].payload)
        
        # PNG 시그니처가 있는지 확인
        if png_signature in reassembled_data:
            start_idx = reassembled_data.find(png_signature)
            if png_end_marker in reassembled_data[start_idx:]:
                end_idx = reassembled_data.find(png_end_marker, start_idx) + len(png_end_marker)
                png_data = reassembled_data[start_idx:end_idx]
                
                output_file = f"extracted_secret_{ip_id}.png"
                with open(output_file, 'wb') as f:
                    f.write(png_data)
                print(f"PNG extracted and saved as {output_file}")
    
    # 두번째 방법: HTTP/TCP 스트림에서 PNG 찾기
    all_data = b''
    tcp_streams = {}
    
    for packet in packets:
        if TCP in packet and Raw in packet:
            stream_id = (packet[IP].src, packet[IP].dst, packet[TCP].sport, packet[TCP].dport)
            if stream_id not in tcp_streams:
                tcp_streams[stream_id] = b''
            tcp_streams[stream_id] += packet[Raw].load
    
    # 각 TCP 스트림에서 PNG 찾기
    for stream_id, stream_data in tcp_streams.items():
        if png_signature in stream_data:
            start_idx = stream_data.find(png_signature)
            if png_end_marker in stream_data[start_idx:]:
                end_idx = stream_data.find(png_end_marker, start_idx) + len(png_end_marker)
                png_data = stream_data[start_idx:end_idx]
                
                src_ip, dst_ip, src_port, dst_port = stream_id
                output_file = f"extracted_from_stream_{src_ip}_{src_port}_to_{dst_ip}_{dst_port}.png"
                with open(output_file, 'wb') as f:
                    f.write(png_data)
                print(f"PNG extracted from TCP stream and saved as {output_file}")
    
    # 세번째 방법: 'Secret.png'라는 문자열이 포함된 패킷 검색
    content_packets = []
    
    for packet in packets:
        # 패킷에 Raw 레이어가 있고 그 안에 'Secret.png'가 포함되어 있는지 확인
        if Raw in packet:
            raw_data = packet[Raw].load
            if b'Secret.png' in raw_data or 'Secret.png' in str(raw_data):
                content_packets.append(packet)
    
    if content_packets:
        print(f"Found {len(content_packets)} packets containing 'Secret.png' reference")
        
        # HTTP 응답 패킷 검색
        for i, packet in enumerate(content_packets):
            raw_data = packet[Raw].load
            
            # HTTP 응답 헤더와 PNG 데이터 분리
            if b'HTTP/' in raw_data and b'Content-Type: image/png' in raw_data:
                header_end = raw_data.find(b'\r\n\r\n') + 4
                png_data = raw_data[header_end:]
                
                # PNG 시그니처 확인
                if png_data.startswith(png_signature):
                    output_file = f"Secret_from_http_{i}.png"
                    with open(output_file, 'wb') as f:
                        f.write(png_data)
                    print(f"Extracted Secret.png from HTTP response, saved as {output_file}")
    


if __name__ == "__main__":
    pcap_file = r"C:\Users\gram\Desktop\shark.pcapng"  # 파일 경로 설정
    extract_png_from_pcap(pcap_file)

```

이렇게 코드를 작성해서 추출해보았지만 여전히 수동으로 얻어낸 png 파일과 같이 손상된 파일만이 추출된다.
