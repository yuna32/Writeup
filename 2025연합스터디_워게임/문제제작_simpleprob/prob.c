#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void give_flag() {
    FILE *fp;
    char flag[128];

    fp = fopen("flag.txt", "r");
    if (fp == NULL) {
        printf("Error: flag.txt file not found!\n");
        exit(1);
    }

    fgets(flag, sizeof(flag), fp);
    printf("Here is your flag: %s\n", flag);
    fclose(fp);
}

void vuln() {
    char buffer[32];
    printf("Enter your name: ");
    gets(buffer); 
    printf("Hello, %s!\n", buffer);
}

int main(void) {
    printf("Welcome to the simple buffer overflow challenge.\n");
    vuln();
    return 0;
}