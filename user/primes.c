//
// Created by 王康 on 2022/11/20.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
primes()
{
    int buf;
    int prime;
    int p2cPipe[2];
    int pid;

    if(read(0, &buf, sizeof(int)) == 0) {
        exit(0);
    }
    prime = buf;
    printf("prime %d\n", prime);

    pipe(p2cPipe);
    pid = fork();

    if(pid == 0) {
        close(p2cPipe[1]);
        close(0);
        dup(p2cPipe[0]);
        primes();
        exit(0);
    } else {
        while (read(0, &buf, sizeof(int)) > 0) {
            if (buf % prime == 0) {
                continue;
            }
            close(p2cPipe[0]);
            int temp = buf;
            write(p2cPipe[1], &temp, sizeof(int));
        }
        close(p2cPipe[1]);
        wait(0);
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    int p2cPipe[2];
    int pid;

    if (argc != 1) {
        fprintf(2, "primes: input too long");
        exit(1);
    }

    pipe(p2cPipe);
    pid = fork();

    if (pid == 0) {
        close(p2cPipe[1]);
        close(0);
        dup(p2cPipe[0]);
        primes();
        exit(0);
    } else {
        close(p2cPipe[0]);
        for(int i = 2; i < 35; i++) {
            write(p2cPipe[1], &i, sizeof(int));
        }
        close(p2cPipe[1]);
        wait((int*)0);
    }

    exit(0);
}