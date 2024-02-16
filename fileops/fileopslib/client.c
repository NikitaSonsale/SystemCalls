#include <stdio.h>
#include <stdlib.h>
#include "file_ops.h"
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int ret;
    if(argc != 2)
    {
        puts("usage : ./client <fileName>");
        return EXIT_FAILURE;
    }
    int fd = creatFile(argv[1], S_IRWXU);
    close(fd);

    ret = writeFile(argv[1]);
    if(ret != 0)
    {
        perror("writeFile() failed");
        return EXIT_FAILURE;
    }

    ret = readFile(argv[1]);
    if(ret != 0)
    {
        perror("readFile() failed");
        return EXIT_FAILURE;
    }

    ret = chOwner(argv[1], "root");
    if(ret != 0)
    {
        perror("chOwner() failed");
        return EXIT_FAILURE;
    }

    ret = changeMod(argv[1], 0777);
    if(ret != 0)
    {
        perror("changeMod() failed");
        return EXIT_FAILURE;
    }
    return 0;
}