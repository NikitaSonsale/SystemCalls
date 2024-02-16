#include <stdio.h>
#include <stdlib.h>
#include "vectorReadWrite.h"

int main(int argc, char *argv[]) 
{   
    int ret;

    if (argc != 2) 
    {
        printf("Usage: ./a.out <filename>\n");
        return EXIT_FAILURE;
    }

    ret = positionedRead(argv[1]);
    if(ret != 0)
    {
        return EXIT_FAILURE;
    }

    ret = positionedWrite(argv[1]);
    if(ret != 0)
    {
        return EXIT_FAILURE;
    }

    ret = vectorPositionedRead(argv[1]);
    if(ret != 0)
    {
        return EXIT_FAILURE;
    }

    ret = vectorPositionedWrite(argv[1]);
    if(ret != 0)
    {
        return EXIT_FAILURE;
    }

    return 0;
}