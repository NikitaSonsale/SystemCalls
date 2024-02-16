#include <stdio.h>
#include <stdlib.h>
#include "DirTree.h"

int main(int argc, char* argv[])
{
    struct arguments *arg = NULL;
    arg = (struct arguments*)malloc(sizeof(arg));
    if(arg == NULL)
    {
        printf("Failed to allocate memory to structure\n");
        exit(1);
    }

    getArguments(arg, argc, argv);
    createDirTree(arg);

    free(arg);
    arg == NULL;
    return 0;
}