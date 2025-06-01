Dreamhack secure mail
====================

![image](https://github.com/user-attachments/assets/81477549-da0a-45dc-8a10-0cf9fa3134ad)

생년월일로 이루어진 비밀번호를 알아내는 문제다.

![image](https://github.com/user-attachments/assets/23a0fc5b-ac8d-4449-bfc1-500199b72c84)

html 코드는 위와 같다. 뭔가 암호화, 난독화가 되어있어서 이미지 데이터가 굉장히 길다.

일단 Confirm 버튼을 눌렀을 때 실행되는 함수인 _0x9a220을 좀 관찰해봤다.


```javascript
function _0x9a220(_0x30bf04) {
    var _0x540d50 = _0x225843,
        _0x2ee89c = Array[_0x540d50('0x99', '3itY')](_0x3eebe5(_0x30bf04, null, raw = !![]))[_0x540d50('0xe2', 'G(su')](_0x66324e => _0x66324e['charCodeAt']()),
        _0x2fef58 = new _0x58829a['_0x14c3a3'][(_0x540d50('0x29', 'd%09'))](_0x2ee89c, _0x2ee89c);, dfbora = _0x2fef58['decrypt'](file), odradurs1 = '';
    for (var _0x302add = 0x0; _0x302add < dfbora[_0x540d50('0xde', '@%wH')]; _0x302add++) odradurs1 += String[_0x540d50('0x2', 'dARo')](dfbora[_0x302add]);
    if (_0x3eebe5(odradurs1, null, raw = !![]) != _0x540d50('0x55', 'dGZC')) return alert('Wrong'), ![];
    return document['write'](_0x540d50('0x66', 'AZ$r') + odradurs1 + '\x22>'), !![];
}
```


난독화되어있어서 어렵긴 한데 _0x30bf04 가 6자리의 생년월일이고 _0x3eebe5 함수의 인자로 전달되고 있다는 것은 볼 수 있다. 


![image](https://github.com/user-attachments/assets/9f497653-f950-444a-9d96-14e67de56251)

실제로 자바스크립트 콘솔에서 _0x3eebe5 함수를 직접 호출해보면 **16개**의 문자로 이루어진 암호화된 값이 나오는 것을 볼 수 있다.


이제 _0x3eebe5 함수가 어떤 방법으로 암호화를 수행하는지 알아야한다. 함수 내부를 좀 보면


```javascript
function _0x3170e2(_0x5d5d81, _0x5ccd92, _0x218d2d, _0x52cad5, _0x294fa0, _0x36ec4d, _0x4c8e41) {
    return _0x388613(_0x5ccd92 & _0x218d2d | ~_0x5ccd92 & _0x52cad5, _0x5d5d81, _0x5ccd92, _0x294fa0, _0x36ec4d, _0x4c8e41);
}

function _0x55dc4e(_0x37d6a2, _0x37bcb0, _0x277505, _0x4d37a8, _0x16d1ec, _0x579718, _0x4ef6d1) {
    return _0x388613(_0x37bcb0 & _0x4d37a8 | _0x277505 & ~_0x4d37a8, _0x37d6a2, _0x37bcb0, _0x16d1ec, _0x579718, _0x4ef6d1);
}

function _0x182ec0(_0x317f89, _0x389f9d, _0x5a88f0, _0x30ac42, _0x1dc07a, _0x5a6689, _0x27c5cb) {
    return _0x388613(_0x389f9d ^ _0x5a88f0 ^ _0x30ac42, _0x317f89, _0x389f9d, _0x1dc07a, _0x5a6689, _0x27c5cb);
}

function _0x5527b8(_0x56c591, _0x482632, _0x62392f, _0x4a3e73, _0x2cc931, _0x55c5cd, _0x90e3b6) {
    return _0x388613(_0x62392f ^ (_0x482632 | ~_0x4a3e73), _0x56c591, _0x482632, _0x2cc931, _0x55c5cd, _0x90e3b6);
}
```

라는 구조가 보인다. 뭔가 중요한 거 같긴한데 잘 모르겠어서 지피티한테 물어봐서 알아냈다.

결론부터 말하자면 **md5 해시화** 수행하는 함수이다.

#### 1. `_0x3170e2` 함수

```javascript
function _0x3170e2(_0x5d5d81, _0x5ccd92, _0x218d2d, _0x52cad5, _0x294fa0, _0x36ec4d, _0x4c8e41) {
    return _0x388613(_0x5ccd92 & _0x218d2d | ~_0x5ccd92 & _0x52cad5, _0x5d5d81, _0x5ccd92, _0x294fa0, _0x36ec4d, _0x4c8e41);
}
```

* **핵심 비트 연산:** `_0x5ccd92 & _0x218d2d | ~_0x5ccd92 & _0x52cad5`
* **해석:** 이 연산은 `(B AND C) OR (NOT B AND D)` 형태이다.
* 이는 **MD5 해시 알고리즘의 F 함수 (라운드 1)**와 동일하다.
    * MD5 F 함수: `(X & Y) | (~X & Z)`

#### 2. `_0x55dc4e` 함수

```javascript
function _0x55dc4e(_0x37d6a2, _0x37bcb0, _0x277505, _0x4d37a8, _0x16d1ec, _0x579718, _0x4ef6d1) {
    return _0x388613(_0x37bcb0 & _0x4d37a8 | _0x277505 & ~_0x4d37a8, _0x37d6a2, _0x37bcb0, _0x16d1ec, _0x579718, _0x4ef6d1);
}
```

* **핵심 비트 연산:** `_0x37bcb0 & _0x4d37a8 | _0x277505 & ~_0x4d37a8`
* **해석:** 이 연산은 `(B AND D) OR (C AND NOT D)` 형태이다. 
* **MD5 해시 알고리즘의 G 함수 (라운드 2)**와 유사하다.
    * MD5 G 함수: `(X & Z) | (Y & ~Z)`

#### 3. `_0x182ec0` 함수

```javascript
function _0x182ec0(_0x317f89, _0x389f9d, _0x5a88f0, _0x30ac42, _0x1dc07a, _0x5a6689, _0x27c5cb) {
    return _0x388613(_0x389f9d ^ _0x5a88f0 ^ _0x30ac42, _0x317f89, _0x389f9d, _0x1dc07a, _0x5a6689, _0x27c5cb);
}
```

* **핵심 비트 연산:** `_0x389f9d ^ _0x5a88f0 ^ _0x30ac42`
* **해석:** 이 연산은 `(B XOR C XOR D)` 형태이다.
* **MD5 해시 알고리즘의 H 함수 (라운드 3)**와 동일하다.
    * MD5 H 함수: `X XOR Y XOR Z`

#### 4. `_0x5527b8` 함수

```javascript
function _0x5527b8(_0x56c591, _0x482632, _0x62392f, _0x4a3e73, _0x2cc931, _0x55c5cd, _0x90e3b6) {
    return _0x388613(_0x62392f ^ (_0x482632 | ~_0x4a3e73), _0x56c591, _0x482632, _0x2cc931, _0x55c5cd, _0x90e3b6);
}
```

* **핵심 비트 연산:** `_0x62392f ^ (_0x482632 | ~_0x4a3e73)`
* **해석:** 이 연산은 `C XOR (B OR NOT D)` 형태이다.
* **알고리즘 연관성:** 이는 **MD5 해시 알고리즘의 I 함수 (라운드 4)**와 동일하다.
    * MD5 I 함수: `Y XOR (X | ~Z)`
 


따라서 유저가 입력한 6자리 키를 md5 해시화를 통해 16바이트로 변형한 후 _0x2fef58 = new _0x58829a['_0x14c3a3'][(_0x540d50('0x29', 'd%09'))](_0x2ee89c, _0x2ee89c); 부분을 통해서 _0x58829a['_0x14c3a3'] 클래스를 생성하고 있음을 알 수 있다.




![image](https://github.com/user-attachments/assets/4c1c6129-4c65-4cbb-914a-9184f7e106b4)

_0x58829a['_0x14c3a3'] 클래스를 살펴보면 뭔가 굉장히 많은 값들이 보인다.


잘 모르겠어서 지피티를 이용해보니 **AES 암호화** 와 연관이 있을 가능성이 매우 크다고 한다.

1.  **`_0x49b89d`, `_0x51671c`, `_0xd3858f` (S-Box 및 역 S-Box):**
    * **AES의 S-Box (Substitution Box) 및 역 S-Box (Inverse S-Box)**일 가능성이 매우 높다. S-Box는 바이트 대치(SubBytes) 단계에서 사용되며, 각 바이트를 비선형적으로 다른 바이트로 대체하는 역할을 한다. 역 S-Box는 복호화 시 사용된다.
    * 예를 들어, `_0x49b89d`는 `[0x1, 0x2, ..., 0xc5, 0x91]`와 같이 0부터 255까지의 모든 바이트 값을 셔플(shuffle)하여 저장하는 S-Box의 일반적인 형태이다.

2.  **`_0x3a03e0`, `_0x277086`, `_0x3ca025`, `_0x46c373` (MixColumns 변환):**
    * 이 배열들은 `0xc66363a5`, `0xf87c7c84` 등과 같이 32비트(4바이트) 값으로 구성되어 있습니다.
    * 이는 **AES의 MixColumns(열 섞기) 단계에서 사용되는 미리 계산된 테이블**일 가능성이 매우 높다. MixColumns 단계는 4바이트 워드(word)를 갤로아 필드(Galois Field, GF(2^8)) 연산을 통해 섞는다. 
    * 각 배열의 이름 패턴(`_0x3a03e0`, `_0x277086`, `_0x3ca025`, `_0x46c373`)은 각기 다른 바이트 위치(예: `t0`, `t1`, `t2`, `t3` 또는 `Te0`, `Te1`, `Te2`, `Te3` 등)에 대한 MixColumns 테이블을 나타낼 수 있다.
    * 예를 들어, `0xc66363a5`는 `0x63`을 어떤 행렬에 곱한 결과의 4바이트 값일 수 있다.

3.  **`_0x34f680`, `_0x1a49a6`, `_0x177db2`, `_0x19de07` (역 MixColumns 변환):**
    * 이 배열들 역시 32비트 값으로 구성되어 있다.
    * 이는 **AES 복호화 시 사용되는 역 MixColumns(Inverse MixColumns) 단계의 미리 계산된 테이블**일 가능성이 높다. 역 MixColumns는 암호화 시 MixColumns에서 수행된 연산을 되돌리는 역할을 한다.

4.  **`_0x4a75ad`, `_0xcf7037`, `_0x1673c5`, `_0x2e1152` (라운드 상수? 또는 키 확장 관련?):**
    * 이 배열들은 첫 번째 값이 `0x0`으로 시작하는 것이 특징이다.
    * 이들은 **AES의 키 확장(Key Expansion) 과정에서 사용되는 라운드 상수(Round Constants)**일 수도 있고, 다른 내부적인 테이블일 수도 있다. AES는 라운드마다 다른 키를 사용하며, 이 키들은 원래 키에서 파생된다.
  



일단 그러면 **입력한 생년월일을 md5해 key와 iv로 사용해 암호화 된 보안메일 데이터를 AES로 복호화** 한다고 가정하고 마저 풀어보겠다.


코드를 끝부분으로 쭉 내려보면


![image](https://github.com/user-attachments/assets/b460e3a9-8c39-4949-8a41-cfb422044995)

_0x3eebe5(odradurs1, null, raw = !![]) != _0x540d50('0x55', 'dGZC') 에서 복호화 된 데이터의 md5 해시를 다시 구해 특정 값과 같은지 비교해 Wrong을 alert할지 복호화된 메일 내용을 출력할지 비교한다는 것을 알 수 있다.


![image](https://github.com/user-attachments/assets/63fe76be-4ab0-4631-bfca-a8b3d448203c)

보안메일 출력을 return document['write'](_0x540d50('0x66', 'AZ$r') + odradurs1 + '\x22>'), !![]; 구문을 이용해 하는 것을 확인할 수 있는데 _0x540d50('0x66', 'AZ$r') 문자열은 호출해보면 **<img src="** 임을 알 수 있다.

따라서 복호화 된 보안메일 데이터는 이미지 데이터임을 추측할 수 있다.




복호화에 사용되는 데이터가 6자리이며 생년월일로 구성되어있기 때문에 00-99- 01-12 - 01-31 의 범위 안에 키가 존재한다. 경우의 수가 충분히 작기 때문에 브루트포싱으로 풀 수 있다.

복호화 된 보안메일 데이터는 이미지 데이터이므로 

![image](https://github.com/user-attachments/assets/bd11a6a6-9be6-4948-92ed-d72edeacfd58)

html 내에 첨부되어 있는 이미지처럼 (data:) 로 시작할 것이다. 

따라서 맨 앞의 16바이트만 복호화 해 (data:)가 나오는지만 검사하면 된다.

코드를 써보면

```python
from Crypto.Cipher import AES
from hashlib import md5

encrypted_mail = bytes([0x68,0xda,0xe1,0x1b,0x20,0x6e,0xbc,0x1f,0x1,0xff,0x6f,0x4,0x89,0x8a,0x6e,0x1d])

for yy in range(100):
    for mm in range(1, 13):
        for dd in range(1, 32):
            yymmdd = "%02d%02d%02d" % (yy, mm, dd)
            key = md5(yymmdd.encode('utf-8')).digest() 
            
            dat = AES.new(key, AES.MODE_CBC, key).decrypt(encrypted_mail)

            if dat.startswith(b"data:"): 
                print(yymmdd)
                exit()
```


주요 부분들을 보면

1.  **`for yy in range(100):`**
    **`for mm in range(1, 13):`**
    **`for dd in range(1, 32):`**:
    * 세 개의 중첩된 `for` 루프는 가능한 모든 날짜 조합을 생성한다.
        * `yy`: 0부터 99까지의 년도 
        * `mm`: 1부터 12까지의 월
        * `dd`: 1부터 31까지의 일


2.  **`key = md5(yymmdd).digest()`**:
    * 생성된 `yymmdd` 문자열을 MD5 해시 함수의 입력으로 사용한다.
    * `.digest()`는 MD5 해시 값을 16바이트(128비트)의 바이너리 데이터로 반환한다. 이 16바이트 데이터가 AES 암호화에 사용될 대칭 키가 된다. 

3.  **`dat = AES.new(key, AES.MODE_CBC, key).decrypt(encrypted_mail)`**:
    * **`AES.new(key, AES.MODE_CBC, key)`**: AES 암호화 객체를 생성한다.
        * 첫 번째 `key`: AES 암호화에 사용할 비밀 키이다. MD5 해시를 통해 생성된 16바이트 키
        * 두 번째 `key`: CBC 모드에서는 초기화 벡터(IV)가 필요하다. 이 코드에서는 키 자체를 IV로 사용하고 있다.
    * **`.decrypt(encrypted_mail)`**: `encrypted_mail`에 저장된 암호화된 데이터를 복호화한다. 복호화된 결과는 `dat` 변수에 저장된다.

4.  **`if dat.startswith("data:"):`**:
    * 복호화된 데이터(`dat`)가 "data:"라는 문자열로 시작하는지 확인한다.
  


![image](https://github.com/user-attachments/assets/732b6955-7ca1-45a0-b520-8a1fbf45d0ee)

올바른 생년월일은 **960229**

![image](https://github.com/user-attachments/assets/39181688-a840-4ae9-b1df-9cbcfb3e6ee5)

이렇게 플래그를 얻을 수 있다. 
