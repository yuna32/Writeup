dreamhack Textbook_DH writeup
==============


## 1. challenge.py 분석

```python
#!/usr/bin/python3
from Crypto.Util.number import getPrime
from Crypto.Util.Padding import pad, unpad
from Crypto.Cipher import AES
import hashlib
import random

class Person(object):
    def __init__(self, p):
        self.p = p
        self.g = 2
        self.x = random.randint(2, self.p - 1)
    
    def calc_key(self):
        self.k = pow(self.g, self.x, self.p)
        return self.k

    def set_shared_key(self, k):
        self.sk = pow(k, self.x, self.p)
        aes_key = hashlib.md5(str(self.sk).encode()).digest()
        self.cipher = AES.new(aes_key, AES.MODE_ECB)

    def encrypt(self, pt):
        return self.cipher.encrypt(pad(pt, 16)).hex()

    def decrypt(self, ct):
        return unpad(self.cipher.decrypt(bytes.fromhex(ct)), 16)

flag = open("flag", "r").read().encode()
prime = getPrime(1024)
print(f"Prime: {hex(prime)}")
alice = Person(prime)
bob = Person(prime)

alice_k = alice.calc_key()
print(f"Alice sends her key to Bob. Key: {hex(alice_k)}")
print("Let's inturrupt !")
alice_k = int(input(">> "))
if alice_k == alice.g:
    exit("Malicious key !!")
bob.set_shared_key(alice_k)

bob_k = bob.calc_key()
print(f"Bob sends his key to Alice. Key: {hex(bob_k)}")
print("Let's inturrupt !")
bob_k = int(input(">> "))
if bob_k == bob.g:
    exit("Malicious key !!")
alice.set_shared_key(bob_k)

print("They are sharing the part of flag")
print(f"Alice: {alice.encrypt(flag[:len(flag) // 2])}")
print(f"Bob: {bob.encrypt(flag[len(flag) // 2:])}")
```

일단 주어진 challenge.py를 분석해보자. 

여기서는 디피-헬먼 키 교환을 이용하고 있다.   
Alice와 Bob은 서로의 비밀 키를 모르는 상태에서 공개된 값만을 이용해 동일한 공유 비밀 키를 알 수 있다. 

* 공개 값: $p$ (소수), $g = 2$

* Alice의 공개 키: $k_{alice} = g^{x_{alice}} \pmod p$

* Bob의 공유 비밀 계산: $sk_{bob} = (k_{alice})^{x_{bob}} \pmod p$    

* 공유 비밀키를 구하면 이를 MD5 해시 처리하여 16바이트 AES 키로 사용


코드 중간에 input()을 통해 키를 가로채고 수정할 수 있는 부분이 구현되어 있는데 이것이 바로 중간자 공격을 수행하라는 표시이다. 


## 2. 중간자 공격(MITM) 시나리오

공격자의 입장에서는 Alice와 Bob 사이에서 전달되는 공개 키()를 마음대로 바꿀 수 있다. `input(">> ")` 부분에 특정한 값을 넣어 **두 사람의 공유 비밀()을 공격자가 아는 값으로 강제 고정**시켜야 한다.

### 공격 전략: Parameter Injection

코드에서 `g`(2)를 넣는 것은 막혀 있지만 **`1`**은 막혀 있지 않다.

1. Alice가 키를 보낼 때: 1을 입력, Bob에게 보내기
* Bob이 계산하는 공유 비밀키: $sk_{bob} = 1^{x_{bob}} \pmod p = \mathbf{1}$


2. Bob이 키를 보낼 때: 다시 1을 입력, Alice에게 보내기
* Alice가 계산하는 공유 비밀: $sk_{alice} = 1^{x_{alice}} \pmod p = \mathbf{1}$


3. 결과: Alice와 Bob은 모두 공유 비밀 키가 1이라고 믿게 된다.

---

## 3. 플래그 탈취 

이제 Alice와 Bob이 사용할 AES 키를 직접 계산할 수 있다.

1. **공유 키 확인:** $sk = 1$
2. **AES 키 생성:** `hashlib.md5(b"1").digest()`로 16바이트 키 얻기.
3. **복호화:**
* 출력된 Alice의 암호문을 위에서 만든 키로 AES-ECB 복호화
* 출력된 Bob의 암호문을 같은 방식으로 복호화
* 두 결과를 합치면 전체 flag가 완성



## 3. 익스플로잇 코드

```python
from pwn import *
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
import hashlib

# 1. 서버 연결
p = remote("host3.dreamhack.games", 20278)

# 2. Prime 값 수신
p.recvuntil(b"Prime: ")
prime_str = p.recvline().strip().decode()
prime = int(prime_str, 16)
log.info(f"Prime: {hex(prime)}")

# 3. Alice의 키 가로채기 및 '1' 주입
p.recvuntil(b"Alice sends her key to Bob. Key: ")
alice_k = int(p.recvline().strip(), 16)
log.info(f"Intercepted Alice's Key: {hex(alice_k)}")

p.sendlineafter(b">> ", b"1") # Bob에게 1을 보냄 -> Bob의 sk는 1이 됨

# 4. Bob의 키 가로채기 및 '1' 주입
p.recvuntil(b"Bob sends his key to Alice. Key: ")
bob_k = int(p.recvline().strip(), 16)
log.info(f"Intercepted Bob's Key: {hex(bob_k)}")

p.sendlineafter(b">> ", b"1") # Alice에게 1을 보냄 -> Alice의 sk는 1이 됨

# 5. 암호문 수신
p.recvuntil(b"Alice: ")
alice_ct = p.recvline().strip().decode()
p.recvuntil(b"Bob: ")
bob_ct = p.recvline().strip().decode()

log.info(f"Alice's Ciphertext: {alice_ct}")
log.info(f"Bob's Ciphertext: {bob_ct}")

# 6. 복호화 (공유 비밀 sk를 1로 강제했으므로)
shared_secret = "1"
aes_key = hashlib.md5(shared_secret.encode()).digest()
cipher = AES.new(aes_key, AES.MODE_ECB)

try:
    part1 = unpad(cipher.decrypt(bytes.fromhex(alice_ct)), 16)
    part2 = unpad(cipher.decrypt(bytes.fromhex(bob_ct)), 16)
    
    flag = part1 + part2
    print("-" * 30)
    print(f"FLAG : {flag.decode()}")
    print("-" * 30)
except Exception as e:
    log.error(f"Decryption failed: {e}")

p.close()
```

이 공격이 성공하는 이유는 디피-헬먼 프로토콜 자체에 상대방이 누구인지 확인하는 Authentication 절차가 없기 때문이다.



## 4. 실행 결과

<img width="990" height="362" alt="image" src="https://github.com/user-attachments/assets/c3ecb258-465a-4573-914d-03ead7de3f16" />



