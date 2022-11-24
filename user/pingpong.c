//
// Created by 王康 on 2022/11/20.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int parentPipe[2];
    int childPipe[2];
    pipe(parentPipe);
    pipe(childPipe);

    int pid = fork();
    if (pid == 0) {
        int buf[1];
        close(parentPipe[1]);
        close(childPipe[0]);
        read(parentPipe[0], buf, 1);
        printf("%d: received ping\n", getpid());
        close(parentPipe[0]);
        write(childPipe[1], " ", 1);
        close(childPipe[1]);
        exit(0);
    } else {
        int buf[1];
        close(parentPipe[0]);
        close(childPipe[1]);
        write(parentPipe[1], " ", 1);
        close(parentPipe[1]);
        read(childPipe[0], buf, 1);
        printf("%d: received pong\n", getpid());
        close(childPipe[0]);
        exit(0);
    }
    exit(0);
}