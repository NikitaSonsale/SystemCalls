#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "concurrentDirTreeWorkload.h"

int main(int argc, char *argv[])
{
    pthread_t threads[NO_OF_THREADS];
    int ret;
    int i;
    
    struct arguments *arg = NULL;
    arg = (struct arguments*)malloc(sizeof(arg));
    if(arg == NULL)
    {
        printf("Failed to allocate memory to structure\n");
        exit(1);
    }

    ret = getArguments(arg, argc, argv);
    if(ret != 0)
    {
        return EXIT_FAILURE;
    }

    ret = createDirTree(arg);
    if(ret != 0)
    {
        return EXIT_FAILURE;
    }

    for (i = 0; i < NO_OF_THREADS; i++) 
    { 
        if (pthread_create(&threads[i], NULL, metadataOps, arg) != 0) 
        { 
            free(arg);
            arg = NULL;
            perror("Error in creating thread"); 
            return EXIT_FAILURE; 
        } 
    } 
    
    for (i = 0; i < NO_OF_THREADS; i++) 
    { 
        if (pthread_join(threads[i], NULL) != 0)
        {
            free(arg);
            arg = NULL;
            perror("Error in waiting for threads");
            return EXIT_FAILURE;
        } 
    }

    free(arg);
    arg = NULL;

    return 0;
}
