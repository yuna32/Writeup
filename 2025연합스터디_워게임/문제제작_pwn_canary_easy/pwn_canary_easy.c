#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void banner(void){
    puts("=== Canary & ROP Playground (easy) ===");
    puts("1) leak canary");
    puts("2) overflow");
    puts("3) exit");
}

static void leak_canary(void){
    char buf[64];
    puts("[+] say something (<=64 bytes):");
    ssize_t n = read(0, buf, sizeof(buf));
    if (n <= 0) exit(1);
    puts("[+] echo with slight over-read (80 bytes) ...");
    write(1, buf, 80);
    puts("");
}

static void vuln(void){
    char buf[64];
    puts("[+] overflow me now:");
    read(0, buf, 256);
    puts("done");
}

int main(void){
    setvbuf(stdin,  NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    alarm(120);

    while (1){
        banner();
        printf("> ");
        char sel[8] = {0};
        if (read(0, sel, sizeof(sel)-1) <= 0) return 0;
        switch (atoi(sel)){
            case 1: leak_canary(); break;
            case 2: vuln();        break;
            case 3: puts("bye");   return 0;
            default: puts("?");    break;
        }
    }
    return 0;
}


