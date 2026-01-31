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
5. 최종 단계: 알아낸 패스워드로 `/secure/secret?password=...`를 호출하여 `flag.txt`의 내용을 획득한다. 


* **HMAC (Encrypt-then-MAC)**: 암호화된 데이터 뒤에 메시지 인증 코드(MAC)를 붙여, 복호화 전에 데이터가 변조되었는지 먼저 확인하면 조작된 패딩을 통한 공격 시도를 원천 차단할 수 있습니다.

이제 이 분석을 바탕으로 파이썬의 `requests` 모듈을 이용해 **패딩 오라클 자동화 공격 스크립트**를 작성해 보고 싶으신가요? 원하신다면 기초적인 페이로드 구성법을 안내해 드릴 수 있습니다.
