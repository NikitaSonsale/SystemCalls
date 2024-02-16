#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "openDeleteMul.h"

int main(int argc, char* argv[])
{
    int ret;
    pthread_t tid1, tid2;

    char *fileName = getArguments(argc, argv);
    if(fileName == NULL)
    {
        return EXIT_FAILURE;
    }

    if((pthread_create(&tid1, NULL, read_write_data, fileName)) != 0)
    {
        perror("Error to create read_write_data thread");
        return EXIT_FAILURE;
    }

    if((pthread_create(&tid2, NULL, unlink_and_stat_file, fileName)) != 0)
    {
        perror("Error to create unlink_and_stat_file thread");
        return EXIT_FAILURE;
    }

    // run for 2 min
    sleep(THREAD_RUNTIME_SECONDS); 

    if((pthread_cancel(tid1)) != 0)
    {
        perror("pthread_cancel for read_write_data thread");
        return EXIT_FAILURE;
    }

    if((pthread_cancel(tid2)) != 0)
    {
        perror("pthread_cancel for unlink_and_stat_file thread");
        return EXIT_FAILURE;
    }

    if((pthread_join(tid1, NULL)) != 0)
    {
        perror("Error in waiting for read_write_data thread");
        return EXIT_FAILURE;
    }

    if((pthread_join(tid2, NULL)) != 0)
    {
        perror("Error in waiting for unlink_and_stat_file thread");
        return EXIT_FAILURE;
    }

    return 0;
}