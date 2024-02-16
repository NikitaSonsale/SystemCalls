/*
    README - cmd to compile :
             1) sudo su
             2) export LD_LIBRARY_PATH=:<pwd>
             3) gcc client.c -L. libchtime.so
             4) ./a.out -d <DirPath> -p <newPermissions> -o <ownerName>
*/

#include <stdio.h>
#include <stdlib.h>
#include "chAccessTime.h"

int main(int argc, char *argv[])
{
    int ret;
    struct cmdArg *argumentStruct = (struct cmdArg*)malloc(sizeof(struct cmdArg));
    if(argumentStruct == NULL)
    {
        printf("Malloc failed : unable to allocate memory to struct\n");
        return EXIT_FAILURE;
    }
    
    ret = getArguments(argumentStruct, argc, argv);
    if(ret != 0)
    {
        return EXIT_FAILURE;
    }

    ret = traverseDir(argumentStruct);
    if(ret != 0)
    {
        return EXIT_FAILURE;
    }

    free(argumentStruct);
    argumentStruct = NULL;
    
    return 0;
}
