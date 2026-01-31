dreamhack BMP Recovery writeup
=============


```python
with open('flag.bmp', 'rb') as f:
    data = bytearray(f.read())

data[:0x1C] = b'\x00' * 0x1C
data[0x22:0x36] = b'\x00' * 0x14

with open('flag.bmp.broken', 'wb') as f:
    f.write(data)
```

주어지는 py 파일을 보면 0:1C 까지 0으로 채우고 다시 0x22:0x36까지 0으로 채우고 있다. 이 영역을 복구해야 한다는 뜻으로 보인다.


<img width="672" height="162" alt="image" src="https://github.com/user-attachments/assets/5829baaf-ed02-47aa-ac75-823e98afeadd" />


일단 파일 앞부분에 혼자 훼손되지 않은 부분을 확인 가능하다.   

<img width="755" height="100" alt="image" src="https://github.com/user-attachments/assets/fce1c7c3-9a2e-4955-a668-6d36168ea4a7" />

bmp 파일의 헤더 구조를 생각해보면 여기는 Info Header 중 biBitCount (2 bytes) = 픽셀 하나당 비트 수가 올 자리이다. 
일단 24비트 = 3바이트다.   


bmp 파일의 헤더 구조를 생각해서 0으로 채워진 부분에 들어갈 요소들을 정리하면

1. bmp 파일 헤더 시그니처 42 4D
2. 전체 파일 바이트 값
3. 전체 헤더 바이트 값
4. info header 바이트 값
5. 가로 길이
6. 세로 길이
7. bitplane 값 (1)
8. 이미지 데이터 바이트 값

현재 알고 있는 것들은

전체 파일 바이트 크기 = 14309622  
파일 헤더 크기 = 54  
info 헤더 크기 = 40
bitplane = 1
픽셀 당 바이트 값 = 3

이렇게 들어간다. 나머지는 계산해서 찾을 수 있다. 

* 이미지 데이터 크기: 전체 파일 크기 - (파일 헤더 + 인포 헤더)
* 패딩을 포함한 한 줄의 바이트 수: ((가로 길이 * 픽셀 당 바이트 수) + 3) // 4 * 4 (BMP는 성능을 위해 각 행의 크기를 4의 배수로 맞춤)
* 전체 데이터 크기 관계: 한 줄의 바이트 수 * 세로 길이 = 이미지 데이터 크기

실질적으로 생각해 봐야 할 것은 가로 길이값과 세로 길이값이다. 

일단 이미지 데이터 크기 계산 및 가능한 가로/세로 조합을 뽑아내는 코드를 작성한다.


```python
def analyze_bmp_structure(total_size, header_size, bytes_per_pixel):
    # 1. 이미지 데이터 크기 계산
    image_data_size = total_size - header_size
    
    # 2. 전체 픽셀 수 계산 (패딩 제외한 이론적 최대치)
    total_pixels_approx = image_data_size // bytes_per_pixel
    
    print(f"[1] 이미지 데이터 크기: {image_data_size:,} bytes")
    print(f"[2] 대략적인 전체 픽셀 수: {total_pixels_approx:,} pixels\n")
    
    print("[3] 가능한 (가로, 세로) 조합 탐색 (패딩 고려):")
    print("-" * 45)
    print(f"{'가로(W)':>10} | {'세로(H)':>10} | {'화면 비율(W:H)':>12}")
    print("-" * 45)
    
    found_count = 0
    # 합리적인 가로 범위 내에서 탐색 (1 ~ 10,000)
    for w in range(1, 10001):
        # BMP 가로 한 줄의 실제 바이트 크기 (4바이트 정렬 패딩 포함)
        row_size = (w * bytes_per_pixel + 3) // 4 * 4
        
        # 이미지 데이터 크기가 행 크기로 나누어 떨어지면 유효한 조합
        if image_data_size % row_size == 0:
            h = image_data_size // row_size
            aspect_ratio = w / h
            
            # 너무 극단적인 비율을 제외한 주요 조합 출력
            if 0.5 <= aspect_ratio <= 2.5:
                print(f"{w:10,d} | {h:10,d} | {aspect_ratio:12.2f}")
                found_count += 1
                
    print("-" * 45)
    print(f"총 {found_count}개의 유효한 해상도 조합을 찾았습니다 (비율 0.5~2.5 기준).")

# 주어진 값 설정
total_file_size = 14309622
header_size = 54  # 파일 헤더(14) + 인포 헤더(40)
bytes_per_pixel = 3 # 24-bit

analyze_bmp_structure(total_file_size, header_size, bytes_per_pixel)
```


<img width="675" height="460" alt="image" src="https://github.com/user-attachments/assets/c03b40a8-86d1-4cd2-aa22-1f702d1d3fe4" />

일단 이렇게 이미지 데이터 크기 및 가능한 가로세로 조합이 출력되었다.  


하나씩 헥스 에디터로 수정하며 할 수도 있지만 현재 나온 가로세로 조합이 32개라는 많지 않은 숫자이므로 현재까지 구한 헤더 정보+가능한 가로세로조합을 모두 합쳐서 32개의 파일을 생성한 뒤 그 중에 유효한 파일을 찾아낼 것이다.    


```python
import os
import struct

def recover_with_full_header():
    input_filename = 'flag.bmp.broken'
    output_dir = "recovered_full_header"
    
    # 1. 원본 파일 로드 및 데이터 분리
    if not os.path.exists(input_filename):
        print(f"오류: {input_filename} 파일이 없습니다.")
        return

    with open(input_filename, 'rb') as f:
        all_data = f.read()
    
    # 헤더 이후의 순수 픽셀 데이터만 추출 (Offset 54부터 끝까지)
    pixel_data = all_data[54:]
    
    # 기본 정보 설정
    total_file_size = 14309622
    header_size = 54
    image_data_size = total_file_size - header_size # 14,309,568
    
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 2. 구해둔 유효 조합 리스트
    raw_data = """
    1567 | 3042, 1568 | 3042, 1577 | 3024, 1698 | 2808, 1763 | 2704, 1764 | 2704,
    1802 | 2646, 1871 | 2548, 1872 | 2548, 1941 | 2457, 2015 | 2366, 2016 | 2366,
    2027 | 2352, 2028 | 2352, 2183 | 2184, 2184 | 2184, 2351 | 2028, 2352 | 2028,
    2495 | 1911, 2496 | 1911, 2547 | 1872, 2548 | 1872, 2703 | 1764, 2704 | 1764,
    2911 | 1638, 2912 | 1638, 3135 | 1521, 3136 | 1521, 3154 | 1512, 3275 | 1456,
    3276 | 1456, 3397 | 1404
    """
    
    pairs = []
    entries = raw_data.replace('\n', '').split(',')
    for entry in entries:
        if '|' in entry:
            w, h = entry.split('|')
            pairs.append((int(w.strip()), int(h.strip())))

    print(f"새로운 헤더를 생성하여 {len(pairs)}개의 파일을 복구합니다...")

    # 3. 파일 생성 작업
    for i, (w, h) in enumerate(pairs, 1):
        # --- [BITMAP FILE HEADER (14 bytes)] ---
        bfType = b'BM'                         # 파일 타입
        bfSize = struct.pack('<I', total_file_size) # 전체 파일 크기
        bfReserved = struct.pack('<H', 0) * 2  # 예약 영역 (0)
        bfOffBits = struct.pack('<I', 54)      # 데이터 시작 오프셋 (54)
        
        file_header = bfType + bfSize + bfReserved + bfOffBits
        
        # --- [BITMAP INFO HEADER (40 bytes)] ---
        biSize = struct.pack('<I', 40)         # 인포 헤더 크기
        biWidth = struct.pack('<I', w)         # 가로
        biHeight = struct.pack('<I', h)        # 세로
        biPlanes = struct.pack('<H', 1)        # 비트플레인 (1)
        biBitCount = struct.pack('<H', 24)     # 픽셀당 비트 (3바이트=24비트)
        biCompression = struct.pack('<I', 0)   # 압축방식 (0=없음)
        biSizeImage = struct.pack('<I', image_data_size) # 이미지 데이터 크기
        biXPelsPerMeter = struct.pack('<I', 0) # 가로 해상도 (기본값 0)
        biYPelsPerMeter = struct.pack('<I', 0) # 세로 해상도 (기본값 0)
        biClrUsed = struct.pack('<I', 0)       # 사용 색상수 (기본값 0)
        biClrImportant = struct.pack('<I', 0)  # 중요 색상수 (기본값 0)
        
        info_header = biSize + biWidth + biHeight + biPlanes + biBitCount + \
                      biCompression + biSizeImage + biXPelsPerMeter + \
                      biYPelsPerMeter + biClrUsed + biClrImportant
        
        # --- [파일 결합] ---
        full_file_data = file_header + info_header + pixel_data
        
        filename = f"fixed_{w}x{h}.bmp"
        filepath = os.path.join(output_dir, filename)
        
        with open(filepath, 'wb') as f:
            f.write(full_file_data)
            
    print("-" * 50)
    print(f"작업 완료: '{output_dir}' 폴더 내에 정상적인 헤더를 가진 파일들이 생성되었습니다.")

if __name__ == "__main__":
    recover_with_full_header()
```

<img width="780" height="343" alt="image" src="https://github.com/user-attachments/assets/7d2a910b-ce36-4fb0-a1e3-5d92f3c80eaa" />

코드를 실행하면 이렇게 총 32개의 파일들이 복구된다. 


<img width="878" height="263" alt="image" src="https://github.com/user-attachments/assets/c9057e9d-79d5-40b9-97b1-3ca7864c2703" />

 이중 플래그가 명확히 보이는 파일을 찾아내면 된다. 
