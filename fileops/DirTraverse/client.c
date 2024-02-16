#include <stdio.h>
#include "traverseDir.h"

int main(int argc, char *argv[])
{
    char *dirPath = NULL;
    dirPath = getArguments(argc, argv);
    iterateDir(dirPath);
    return 0;
}
