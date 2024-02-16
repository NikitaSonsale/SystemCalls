#include <stdio.h>
#include <stdlib.h>
#include "ReadWrite.h"

int main(int argc, char *argv[])
{
    struct arguments *arg = NULL;
    arg = (struct arguments*)xmalloc(sizeof(arg));

    getArguments(arg, argc, argv);
    creatFile(arg);
    writeFile(arg);
    readFile(arg);
    printOP(arg);

    free(arg);
    arg = NULL;
    return 0;
}

