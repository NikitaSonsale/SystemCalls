#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ReadWrite.h"

void getArguments(struct arguments *arg, int argc, char *argv[])
{
    int option;
    int i;

    if(argc < 2 || argc == 3 || argc == 5 || argc > 7)
    {
        printf("Usage : ./a.out -f <FileName> -s <FileSize> -c <ChunkSize>\n");
        exit(1);
    }

    /*
     *  -f for FileName
     *  -s for sizeOfFile
     *  -c for sizeOfChunk
     */   

    while((option = getopt(argc, argv, "f:s:c:")) != -1)
    {
        switch(option)
        {
            case 'f':
                arg->FileName = optarg;
                break;

            case 's':
                arg->sizeOfFile = atoi(optarg);

                if(arg->sizeOfFile == 0)
                {
                    printf("Give valid size for file\n");
                    exit(1);
                }

                break;

            case 'c':

                arg->sizeOfChunk = atoi(optarg);

                if(arg->sizeOfChunk == 0)
                {
                    printf("Give valid size for chunk to read or write\n");
                    exit(1);
                }

                break;

            default:
                exit(1);
        }
    }

    // To handle non option arguments
    for(i = optind; i < argc; i++)
    {
        printf("Non-option argument %s\n", argv[i]);
        printf("Usage : ./a.out -f <FileName> -s <FileSize> -c <ChunkSize>\n");
        exit(1);
    }
}

void creatFile(struct arguments *arg)
{
    int fd;
    fd = open(arg->FileName, O_CREAT|O_RDWR, S_IRWXU);

    if(fd == -1)
    {
        perror(NULL);
        exit(1);
    }

    close(fd);
}

void writeFile(struct arguments *arg)
{
    int fd;
    int i;
    char *buffer = (char *)xmalloc(sizeof(char) * (arg->sizeOfChunk));
    fd = open(arg->FileName, O_WRONLY);
    if(fd == -1)
    {
        perror(NULL);
        exit(1);
    }


    unsigned long ret;
    for(int j = 0; j < (arg->sizeOfFile); j = j + (arg->sizeOfChunk))
    {
        for(i = 0; i < (arg->sizeOfChunk); i++)
        {
            // fill the buffer with random data
            buffer[i] = ('A' + (rand() % 25));
        }
    
        // writing the buffer into file
		ret = write(fd, buffer, arg->sizeOfChunk);
    	
        if (ret < 0)
    	{
			free(buffer);                                                           
            buffer = NULL;
			close(fd);                                                                 
        	printf("unable to write data in file");
        	exit(1);
    	}
    }

    free(buffer);
    buffer = NULL;
    close(fd);
}

void readFile(struct arguments *arg)
{
    int fd;
    int count;
    char *buffer = (char *)xmalloc(sizeof(char) * (arg->sizeOfChunk));

    fd = open(arg->FileName, O_RDONLY);
    if(fd == -1)
    {
        free(buffer);
        buffer = NULL; 
        perror(NULL);
        exit(1);
    }

    while((count = read(fd, buffer, arg->sizeOfChunk)) > 0)
    {
        // do nothing
    }
    
    free(buffer);
    buffer = NULL;
}

void printOP(struct arguments *arg)
{
    struct stat statFile;
    int i;
    i = stat(arg->FileName, &statFile);
    if(i == 0)
        printf("%s with size %lu is created Successfully\n", arg->FileName, statFile.st_size);
    else
    {
        printf("File Creation Failed\n");
        exit(1);
    }   
}

void *xmalloc(size_t sizeInBytes)
{
    void *p;
    p = malloc(sizeInBytes);
    if(p == NULL)
    {
        printf("Failed to allocate memory\n");
        exit(1);
    }
    return p;    
}
