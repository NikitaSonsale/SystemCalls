#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "DirTree.h"

// function to get arguments from command line
void getArguments(struct arguments* arg, int argc, char *argv[])
{
    int option, i;
    
    if(argc < 2 || argc == 3 || argc == 5 || argc == 7 ||  argc == 9 || argc > 11)
    {
        printf("Usage : ./a.out -n <DirectoryName> -r <NoOfRegularFiles> -m <maxfizeSize> -s <NoOfSubdirectories> -d <DepthOfTree>\n");
        exit(1);
    }

    /*
        -n for name of main directory
        -r for Number of regular files
        -m for Maximum file size
        -s for Number of subdirectories
        -d for Depth of Directory Tree
    */

    while((option = getopt(argc, argv, "n:r:m:s:d:")) != -1)
    {
        switch(option)
        {
            case 'n':
                arg->directoryName = optarg;
                break;
        
            case 'r':
                arg->noOfFiles = atoi(optarg);
                if(arg->noOfFiles == 0)
                {
                    printf("Give valid number for regular files\n");
                    exit(1);
                }
                break;

            case 'm':
                arg->maxFileSize = atoi(optarg);
                if(arg->maxFileSize == 0)
                {
                    printf("Give valid maximum size for regular file\n");
                    exit(1);
                }
        
            case 's':

                arg->noOfSubdirectories = atoi(optarg);

                if(arg->noOfSubdirectories == 0)
                {
                    printf("Give valid number for subdirectories\n");
                    exit(1);
                }

                break;

            case 'd':

                arg->depthOfTree = atoi(optarg);

                if(arg->depthOfTree == 0)
                {
                    printf("Give valid depth for directory tree\n");
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
        printf("Usage : ./a.out -r <NoOfRegularFiles> -s <NoOfSubdirectories> -d <DepthOfTree>\n");
        exit(1);
    }
}


// function to create directory tree
void createDirTree(struct arguments *arg)
{
    createDirectories(arg->directoryName, arg->noOfFiles, arg->noOfSubdirectories, arg->depthOfTree, arg->maxFileSize);
}

// function to create regular files in subdirectories
void createRegFiles(char *dirName, int noOfFiles, int maxFileSize)
{
    int i;
    int fd;
    int fileSize;
    char *regFilePath = (char*)malloc(256 * sizeof(char));
    if(regFilePath == NULL)
    {
        printf("Failed to allocate memory\n");
        exit(1);
    }

    char *buffer = (char*)malloc(maxFileSize * sizeof(char));
    if(buffer == NULL)
    {
        printf("Failed to allocate memory to buffer\n");
        exit(1);
    }

    for (i = 0; i < noOfFiles; i++) 
    {
        // generating different names to regular files
        sprintf(regFilePath, "%s/Regfile%d.txt", dirName, i);
        fd = open(regFilePath, O_CREAT|O_RDWR, S_IRWXU);
        if (fd == -1) 
        {
            perror("open");
            exit(1);
        }

        // to create file with random file size 
        fileSize = rand() % maxFileSize + 1;
        writeFile(regFilePath, fileSize, maxFileSize);
        close(fd);
    }
}

// function to create directories
void createDirectories(char *dirName, int noOfFiles, int noOfSubdir, int depthOfTree, int maxFileSize)
{
    int i;
    char *subdirPath = (char*)malloc(256 * sizeof(char));
    if(subdirPath == NULL)
    {
        printf("Failed to allocate memory\n");
        exit(1);
    }

    if(depthOfTree <= 0)
    {
        return;
    }

    if(mkdir(dirName, 0764) == -1)
    {
        perror(NULL);
        exit(1);
    }

    // creating given no. of regular files
    createRegFiles(dirName, noOfFiles, maxFileSize);

    for(i = 0; i < noOfSubdir; i++)
    {
        // generating different names to subdirectories
        sprintf(subdirPath, "%s/subdir%d", dirName, i);

        // generating files and  subdirectories recursively
        createDirectories(subdirPath, noOfFiles, noOfSubdir, depthOfTree - 1, maxFileSize);   
    }
}

// function to write random data in regular files.
void writeFile(char *fileName, int fileSize, int maxFileSize)
{
    int fd;
    int i;
    char *buffer = (char*)malloc(sizeof(char) * maxFileSize);
    fd = open(fileName, O_WRONLY);
    if(fd == -1)
    {
        perror(NULL);
        exit(1);
    }

    unsigned long ret;
    for(i = 0; i < fileSize; i++)
    {
        // fill the buffer with random data
        buffer[i] = ('A' + (rand() % 25));
    }
    
    // writing the buffer into file
	ret = write(fd, buffer, fileSize);
    if (ret < 0)
    {
		free(buffer);                                                           
        buffer = NULL;
		close(fd);                                                                 
        printf("unable to write data in file");
        exit(1);
    }

    free(buffer);
    buffer = NULL;
    close(fd);
}

