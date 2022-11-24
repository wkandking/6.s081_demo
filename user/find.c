//
// Created by 王康 on 2022/11/20.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char*
check(char *path)
{
    char *buff = (char*)malloc(DIRSIZ + 1);
    char *p;

    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buff, p, strlen(p));
    memset(buff+strlen(p), ' ', DIRSIZ-strlen(p));
    return buff;
}

void
findDescend(char *path, char *findName)
{
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512];
    char *p;
    char *temp;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
            temp = check(path);
            if (strcmp(temp, findName) == 0) {
                printf("%s\n", path);
            }
            free(temp);
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (stat(buf, &st) < 0) {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                findDescend(buf, findName);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    char *temp;
    temp = check(argv[2]);
    if (argc != 3){
        fprintf(2, "find: input error!\n");
        exit(0);
    }
    findDescend(argv[1], temp);
    free(temp);
    exit(0);
}