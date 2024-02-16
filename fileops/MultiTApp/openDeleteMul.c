#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "openDeleteMul.h"

char* getArguments(int argc, char *argv[])
{
    int option, i;
    char *fileName;
    
    if(argc < 2 || argc > 3)
    {
        printf("Usage : ./a.out -n <FileName>\n");
        return NULL;
    }

    //-n for name of file
       
    while((option = getopt(argc, argv, "n:")) != -1)
    {
        switch(option)
        {
            case 'n':
            fileName = optarg;  
                break;
        
            default:
                printf("Invalid flag\n");
        }
    }

    // To handle non option arguments
    for(i = optind; i < argc; i++)
    {
        printf("Non-option argument %s\n", argv[i]);
        printf("Usage : ./a.out -n <FileName>\n");
        return NULL;
    }

    return fileName;
}

void *read_write_data(void *arg)
{
    char *fileName = (char*)arg;
    int fd;
    int ret;

    while (1) 
    {
        fd = open(fileName, O_RDWR | O_CREAT, S_IRWXU);
        if (fd == -1) 
        {
            perror("creatFile() failed in read_write_data");
            continue;
        }
        else
        {
            printf("Thread1 created file %s\n", fileName);
        }
    
        sleep(1);
        ret = writeFile(fd);
        if (ret != 0) 
        {
            perror("Error to write data in read_write_data thread");
            continue;
        } 
    
        lseek(fd, 0, SEEK_SET);
        ret = readFile(fd);
        if (ret != 0) 
        {
            perror("Error to read data in read_write_data thread");
            continue;
        } 

        if((close(fd)) == -1)
        {
            perror("Error to close the file in read_write_data");
            continue;
        }   
    }
}

void *unlink_and_stat_file(void *arg)
{
    char *fileName = (char*)arg;
    struct stat st;
    int ret;
    int fd;

    while(1)
    {
        fd = open(fileName, O_RDONLY);
        if (fd == -1) 
        {
            continue;
        }

        ret = unlink(fileName);
        if (ret == -1) 
        {
            continue;
        } 
        else 
        {
            puts("unlink_and_stat_file thread Deleted file");
        }
            
        ret = stat(fileName, &st);
        if (ret == -1) 
        {
            printf("stat failed : unable to get the metadata as file is deleted by thread2(expected)\n");
            continue;        
        } 

        if((close(fd)) == -1)
        {
            puts("unable to close the file");
            continue;
        }
    }       
}

int writeFile(int fd)
{
    int i;
    unsigned long ret;

    char *buffer = (char*)malloc(sizeof(char) * 1024);
    if(buffer == NULL)
    {
        printf("malloc failed : unable to allocate memory to buffer in writeFile()\n");
        return errno;
    }

    for(i = 0; i < 1024; i++)
    {
        // fill the buffer with random data
        buffer[i] = ('A' + (rand() % 25));
    }
    
    // writing the buffer into file
	ret = write(fd, buffer, 1024);
    if (ret < 0)
    {
		free(buffer);                                                           
        buffer = NULL;                                                               
        printf("write() failed : unable to write data in file in writeFile()\n");
        return errno;
    }

    free(buffer);
    buffer = NULL;

    return 0;
}

int readFile(int fd)
{
    int count;
    
    char *buffer = (char *)malloc(1024*sizeof(char));
    if(buffer == NULL)
    {
        printf("malloc failed : unable to allocate memory to buffer in readFile()\n");
        return errno;
    }
   
    // set the offset to starting of file.
    lseek(fd, 0, SEEK_SET);
   
    if((count = read(fd, buffer, 1024)) > 0)
    {     
        printf("data : %s\n", buffer);
    }
  
    free(buffer);
    buffer = NULL;

    return 0;
}