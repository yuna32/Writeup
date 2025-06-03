#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h> 
#include <sys/user.h>   
#include <sys/wait.h>   
#include <unistd.h>    
#include <errno.h>

void run_debugger(pid_t child_pid);
void run_target(char *program_path, char *argv[]);
void display_registers(pid_t pid);
void display_current_instruction(pid_t pid);
long get_instruction_at_rip(pid_t pid, unsigned long rip);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <대상 프로그램> [인자...]\n", argv[0]);
        return 1;
    }

    // 자식 프로세스 생성
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork 실패");
        return 1;
    }

    if (child_pid == 0) {

        run_target(argv[1], &argv[1]);
    } else {
        // 부모 프로세스 (디버거)
        run_debugger(child_pid);
    }

    return 0;
}

void run_target(char *program_path, char *argv[]) {

    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
        perror("PTRACE_TRACEME 실패");
        exit(1);
    }

    kill(getpid(), SIGSTOP);

    // 대상 프로그램 실행
    execvp(program_path, argv);
    perror("execvp 실패");
    exit(1);
}

// 디버거의 주 로직 (부모 프로세스)
void run_debugger(pid_t child_pid) {
    int status;
    char command[256];

    // 자식 프로세스가 PTRACE_TRACEME와 SIGSTOP 후 스스로 정지할 때까지 대기
    if (waitpid(child_pid, &status, 0) == -1) {
        perror("디버거 초기 waitpid 실패");
        return;
    }

    if (WIFEXITED(status)) {
        printf("대상 프로그램이 즉시 종료되었습니다.\n");
        return;
    }

    printf("SWINGdbg >>> ");
    while (fgets(command, sizeof(command), stdin) != NULL) {
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "c") == 0) {
            // 대상 프로그램 계속 실행
            if (ptrace(PTRACE_CONT, child_pid, NULL, NULL) == -1) {
                perror("PTRACE_CONT 실패");
                break;
            }
        } else if (strcmp(command, "ni") == 0) {
            // 단일 명령어 실행
            if (ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL) == -1) {
                perror("PTRACE_SINGLESTEP 실패");
                break;
            }
        } else if (strcmp(command, "q") == 0) { // 종료 명령
            printf("디버거를 종료합니다.\n");
            ptrace(PTRACE_KILL, child_pid, NULL, NULL); // 자식 프로세스 종료
            break;
        } else {
            printf("알 수 없는 명령: %s\n", command);
        }

        // 자식 프로세스가 다시 정지할 때까지 대기 (continue 또는 singlestep 후)
        if (waitpid(child_pid, &status, 0) == -1) {
            perror("디버거 루프 waitpid 실패");
            break;
        }

        if (WIFEXITED(status)) {
            printf("대상 프로그램이 상태 %d로 종료되었습니다.\n", WEXITSTATUS(status));
            break;
        } else if (WIFSIGNALED(status)) {
            printf("대상 프로그램이 시그널 %d에 의해 종료되었습니다.\n", WTERMSIG(status));
            break;
        } else if (WIFSTOPPED(status)) {
            printf("[+] 자식 프로세스가 시그널 %d에 의해 정지되었습니다.\n", WSTOPSIG(status));
            // 'ni' 명령 후 또는 시그널에 의해 멈춘 경우
            display_current_instruction(child_pid);
            display_registers(child_pid);
        }

        printf("SWINGdbg >>> ");
    }
}

// 레지스터 값을 출력하는 함수
void display_registers(pid_t pid) {
    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
        perror("PTRACE_GETREGS 실패");
        return;
    }

    printf("[+] Registers:\n");
    printf("RAX: 0x%llx\n", regs.rax);
    printf("RBX: 0x%llx\n", regs.rbx);
    printf("RCX: 0x%llx\n", regs.rcx);
    printf("RDX: 0x%llx\n", regs.rdx);
    printf("RSI: 0x%llx\n", regs.rsi);
    printf("RDI: 0x%llx\n", regs.rdi);
    printf("RSP: 0x%llx\n", regs.rsp);
    printf("RBP: 0x%llx\n", regs.rbp);
}

// 현재 명령어를 출력하는 함수 
void display_current_instruction(pid_t pid) {
    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
        perror("RIP를 위한 PTRACE_GETREGS 실패");
        return;
    }

    unsigned long rip = regs.rip;
    long instruction_data = get_instruction_at_rip(pid, rip);

    // 이미지와 같이 RIP와 명령어 바이트 출력
    printf("[+] RIP = 0x%llx | instr = 0x%lx\n", rip, instruction_data);
}

// RIP가 가리키는 메모리 주소에서 명령어 바이트를 읽어오는 함수
long get_instruction_at_rip(pid_t pid, unsigned long rip) {
    long data = ptrace(PTRACE_PEEKTEXT, pid, (void*)rip, NULL);
    if (data == -1 && errno) { // -1이 유효한 데이터일 수 있으므로 errno로 실제 오류 확인
        perror("PTRACE_PEEKTEXT 실패");
    }
    return data;
}