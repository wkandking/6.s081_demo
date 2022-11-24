//
// Created by 王康 on 2022/11/17.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc != 2){
        fprintf(2, "Usage: sleep call input error");
        exit(1);
    }else {
        int tick = atoi(argv[1]);
        sleep(tick);
        exit(0);
    }
}