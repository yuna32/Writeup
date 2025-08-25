#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

static void setup(void) {
    setvbuf(stdin,  NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

static void banner(void) {
    puts("=== OOB Lab (PIE + RELRO + NX, no-canary) ===");
    puts("Hint: OOB read to leak PIE; OOB write for ROP.");
}

__attribute__((noinline))
static void vuln(void) {
    uint64_t arr[8] = {0};      // [0..7] 총 8칸, 경계 밖은 RBP/RIP 영역
    char cmd[8];
    int idx;
    uint64_t val;

    while (1) {
        puts("\n1) read idx");
        puts("2) write idx value");
        puts("3) exit");
        printf("> ");
        if (!fgets(cmd, sizeof(cmd), stdin)) exit(0);

        if (cmd[0] == '1') {
            printf("idx? ");
            if (scanf("%d", &idx) != 1) exit(0);
            int c; while ((c = getchar()) != '\n' && c != EOF);

            uint64_t x = arr[idx];
            printf("arr[%d] = 0x%016lx\n", idx, x);

        } else if (cmd[0] == '2') {
            printf("idx? ");
            if (scanf("%d", &idx) != 1) exit(0);
            printf("value? (hex like 0xdeadbeef)\n> ");
            if (scanf("%lx", &val) != 1) exit(0);
            int c; while ((c = getchar()) != '\n' && c != EOF);

            arr[idx] = val;
            puts("ok");

        } else if (cmd[0] == '3') {
            puts("bye");
            return;
        } else {
            puts("?");
        }
    }
}

int main(void) {
    setup();
    banner();

    puts("warmup");

    vuln();
    return 0;
}
