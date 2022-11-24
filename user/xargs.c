//
// Created by 王康 on 2022/11/21.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAX_LINE 1024

int
main(int argc, char *argv[])
{
    char *callName;
    char *callArgv[MAXARG];
    char buf[MAX_LINE];
    int n, argv_index = argc - 1;
    int isEnd;
    int i;

    callName = argv[1];
    isEnd = 0;

    for (i = 1; i < argc; i++) {
        callArgv[i - 1] = argv[i];
    }
    i = 0;
    while ((n = read(0, &buf[i], sizeof(char))) > 0) {
        if (buf[i] == '\n') {
            isEnd = 1;
            buf[i] = 0;
            callArgv[argv_index++] = buf;
            i = 0;
        }
        if(isEnd) {
            isEnd = 0;
            callArgv[argv_index] = 0;
            argv_index = argc - 1;
            if (fork() == 0) {
                exec(callName, callArgv);
            } else {
                wait((int*)0);
            }
        }
        i++;

    }
    exit(0);
}