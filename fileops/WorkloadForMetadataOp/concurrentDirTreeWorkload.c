#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include "concurrentDirTreeWorkload.h"

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

// function to get arguments from command line
int getArguments(struct arguments* arg, int argc, char *argv[])
{
    int option, i;
    
    if(argc < 2 || argc == 3 || argc == 5 || argc == 7 ||  argc == 9 || argc > 11)
    {
        printf("Usage : ./a.out -n <DirectoryName> -r <NoOfRegularFiles> -m <maxfizeSize> -s <NoOfSubdirectories> -d <DepthOfTree>\n");
        return -1;
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
                    return -1;
                }
                break;

            case 'm':
                arg->maxFileSize = atoi(optarg);
                if(arg->maxFileSize == 0)
                {
                    printf("Give valid maximum size for regular file\n");
                    return -1;
                }
        
            case 's':

                arg->noOfSubdirectories = atoi(optarg);

                if(arg->noOfSubdirectories == 0)
                {
                    printf("Give valid number for subdirectories\n");
                    return -1;
                }

                break;

            case 'd':

                arg->depthOfTree = atoi(optarg);

                if(arg->depthOfTree == 0)
                {
                    printf("Give valid depth for directory tree\n");
                    return -1;
                }

                break;

            default:
                puts("Invalid Flag");
                return -1;
        }
    }

    // To handle non option arguments
    for(i = optind; i < argc; i++)
    {
        printf("Non-option argument %s\n", argv[i]);
        printf("Usage : ./a.out -r <NoOfRegularFiles> -s <NoOfSubdirectories> -d <DepthOfTree>\n");
        return -1;
    }

    return 0;
}

void *metadataOps(void *arg)
{
    struct arguments *argument = (struct arguments*)arg;
    int ret;
    int depth = (rand() % (argument->depthOfTree));
    printf("depth in thread : %d\n", depth);

    char *fullPath = (char*)malloc(sizeof(char) * MAX_PATH);
    if(fullPath == NULL)
    {
        puts("malloc failed : Error to allocate memory to fullPath in metadataOps() thread");
    }

    randomDepth(argument->directoryName, depth, fullPath);
    printf("fullpath in metadataops thread : %s\n", fullPath);

    pthread_mutex_lock(&mtx);
    ret = performOperation(fullPath);
    if(ret != 0)
    {
        perror("perform ops in metadataOps()");
    }
    pthread_mutex_unlock(&mtx);   
}

// function to create directory tree
int createDirTree(struct arguments *arg)
{
    int retV = 0;
    retV = createDirectories(arg->directoryName, arg->noOfFiles, arg->noOfSubdirectories, arg->depthOfTree, arg->maxFileSize);
    if(retV != 0)
    {
        perror("createDirTree");
        return errno;
    }
    return 0;
}

// function to create regular files in subdirectories
int createRegFiles(char *dirName, int noOfFiles, int maxFileSize)
{
    int i;
    int fd;
    int fileSize;
    int ret;
    char *regFilePath = (char*)malloc(256 * sizeof(char));
    if(regFilePath == NULL)
    {
        puts("malloc failed : Error to allocate memory to regFilePath in createRegFiles()");
        return errno;
    }

    char *buffer = (char*)malloc(maxFileSize * sizeof(char));
    if(buffer == NULL)
    {
        free(regFilePath);
        regFilePath = NULL;
        puts("malloc failed : Error to allocate memory to buffer in createRegFiles()");
        return errno;
    }

    for (i = 0; i < noOfFiles; i++) 
    {
        // generating different names to regular files
        sprintf(regFilePath, "%s/Regfile%d.txt", dirName, i);
        fd = open(regFilePath, O_CREAT|O_RDWR, S_IRWXU);
        if (fd == -1) 
        {
            free(regFilePath);
            regFilePath = NULL;
            free(buffer);
            buffer = NULL;
            puts("open failed : Error to create file in createRegFiles()");
            return errno;
        }

        // to create file with random file size 
        fileSize = rand() % maxFileSize + 1;
        ret = writeFile(regFilePath, fileSize, maxFileSize);
        if(ret != 0)
        {
            free(regFilePath);
            regFilePath = NULL;
            free(buffer);
            buffer = NULL;
            close(fd);
            puts("writeFile() failed in createRegFiles");
            return errno;
        }
        close(fd);
    }

    return 0;
}

// function to create directories
int createDirectories(char *dirName, int noOfFiles, int noOfSubdir, int depthOfTree, int maxFileSize)
{
    int i;
    int ret = 0;
    char *subdirPath = (char*)malloc(256 * sizeof(char));
    if(subdirPath == NULL)
    {
        puts("malloc failed : Error to allocate memory to subdirPath in createDirectories()");
        return errno;
    }

    if(depthOfTree <= 0)
    {
        free(subdirPath);
        subdirPath = NULL;
        return 0;
    }

    if(mkdir(dirName, 0764) == -1)
    {
        free(subdirPath);
        subdirPath = NULL;
        puts("mkdir failed : Error to create a dir in createDirectories()");
        return errno;
    }

    // creating given no. of regular files
    ret = createRegFiles(dirName, noOfFiles, maxFileSize);
    if(ret != 0)
    {
        puts("createRegFiles failed in createDirectories()");
        return errno;
    }

    for(i = 0; i < noOfSubdir; i++)
    {
        // generating different names to subdirectories
        sprintf(subdirPath, "%s/subdir%d", dirName, i);

        // generating files and  subdirectories recursively
        createDirectories(subdirPath, noOfFiles, noOfSubdir, depthOfTree - 1, maxFileSize);   
    }
    
}

// function to write random data in regular files.
int writeFile(char *fileName, int fileSize, int maxFileSize)
{
    int fd;
    int i;
    char *buffer = (char*)malloc(sizeof(char) * maxFileSize);
    if(buffer == NULL)
    {
        puts("malloc failed : Error to allocate memory to buffer in writeFile()");
        return errno;
    }

    fd = open(fileName, O_WRONLY);
    if(fd == -1)
    {
        free(buffer);
        buffer = NULL;
        puts("open failed : Error to open file in writeFile()");
        return errno;
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
        puts("write failed : Error in writing random data to the file in writeFile()");
        return errno;
    }

    free(buffer);
    buffer = NULL;
    close(fd);
    return 0;
}

int createFile(char *path)
{
    int fd;
    char *fileName = (char*)malloc(sizeof(char) * MAX_NAME);
    if(fileName == NULL)
    {
        puts("malloc failed : Error to allocate memory to fileName in createFile()");
        return errno;
    }

    sprintf(fileName, "%d.txt", rand());

    char *filePath = (char*)malloc(sizeof(char) * MAX_PATH);
    if(filePath == NULL)
    {
        free(fileName);
        fileName = NULL;
        puts("malloc failed : Error to allocate memory to filePath in createFile()");
        return errno;
    }

    sprintf(filePath, "%s/%s", path, fileName);
    fd = open(filePath, O_CREAT|O_RDWR, S_IRWXU);
    if(fd == -1)
    {
        free(fileName);
        fileName = NULL;
        free(filePath);
        filePath = NULL;
        puts("open failed : unabled to create a file in createFile()");
        return errno;
    }

    free(fileName);
    fileName = NULL;
    free(filePath);
    filePath = NULL;
    close(fd);
    return 0;
}

int deleteFile(char *path)
{
    char *filePath = (char*)malloc(sizeof(char) * MAX_PATH);
    if(filePath == NULL)
    {
        puts("malloc failed : Error to allocate memory to filePath in deleteFile()");
        return errno;
    }

    DIR* dir = opendir(path);
    if (dir == NULL) 
    {
        free(filePath);
        filePath = NULL;
        puts("opendir failed : Error to open the directory in deleteFile()");
        return errno;
    }

    struct dirent* entry;

    // to count total reg. files in given path
    int count = 0;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_REG) 
        {
            count++;
        }
    }

    // if there is no reg. file then return
    if (count == 0) 
    {
        free(filePath);
        filePath = NULL;
        printf("No files to delete.\n");
        closedir(dir);
        return 0;
    }

    int rand_num = rand() % count;
    rewinddir(dir);

    // when index becomes equals to rand_num then delete that reg. file
    int index = 0;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_REG) 
        {
            if (index == rand_num) 
            {
                sprintf(filePath, "%s/%s", path, entry->d_name);
                printf("Deleting file: %s\n", filePath);
                if (remove(filePath) == -1) 
                {
                    free(filePath);
                    filePath = NULL;
                    closedir(dir);
                    puts("remove failed : unable to delete the file in deleteFile");
                    return errno;
                }
                break;
            }
            index++;
        }
    }
    free(filePath);
    filePath = NULL;
    closedir(dir);
    return 0;
}

int createSubdir(char *path)
{
    char *dirName = (char*)malloc(sizeof(char) * MAX_NAME);
    if(dirName == NULL)
    {
        puts("malloc failed : Error to allocate memory to dirName in createSubdir()");
        return errno;
    }

    sprintf(dirName, "dir%d", rand());

    char *dirPath = (char*)malloc(sizeof(char) * MAX_PATH);
    if(dirPath == NULL)
    {
        free(dirName);
        dirName = NULL;
        puts("malloc failed : Error to allocate memory to dirPath in createSubdir()");
        return errno;
    }

    sprintf(dirPath, "%s/%s", path, dirName);
    if((mkdir(dirPath, 0777)) == -1)
    {
        free(dirName);
        dirName = NULL;
        free(dirPath);
        dirPath = NULL;
        puts("mkdir failed : Error to create random subdir");
        return errno;
    }

    free(dirName);
    dirName = NULL;
    free(dirPath);
    dirPath = NULL;
    return 0;
}

int deleteSubdir(char *path)
{
    char *subdirPath = (char*)malloc(sizeof(char) * MAX_PATH);
    if(subdirPath == NULL)
    {
        puts("malloc failed : Error to allocate memory to subdirPath in deleteSubdir()");
        return errno;
    }

    char *cmd = (char*)malloc(sizeof(char) * 500);
    if(cmd == NULL)
    {
        free(subdirPath);
        subdirPath = NULL;
        puts("malloc failed : Error to allocate memory to cmd in deleteSubdir()");
        return errno;
    }

    DIR* dir = opendir(path);
    if (dir == NULL) 
    {
        free(subdirPath);
        subdirPath = NULL;
        free(cmd);
        cmd = NULL;
        puts("opendir failed : Error to open the directory in deleteSubdir()");
        return errno;
    }

    struct dirent* entry;

    // to count total subdir in given path
    int count = 0;             
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
        {
            count++;
        }
    }

    // if there is no subdir then return
    if (count == 0) 
    {
        free(subdirPath);
        subdirPath = NULL;
        free(cmd);
        cmd = NULL;
        printf("No subdirectories to delete.\n");
        closedir(dir);
        return 0;
    }

    int rand_num = rand() % count;
    rewinddir(dir);

    // when index becomes equals to rand_num then delete that subdir
    int index = 0;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
        {
            if (index == rand_num) 
            {
                sprintf(subdirPath, "%s/%s", path, entry->d_name);
                printf("Deleting subdirectory: %s\n", subdirPath);
                sprintf(cmd, "rm -rf %s", subdirPath);
                if (system(cmd) == -1) 
                {
                    free(subdirPath);
                    subdirPath = NULL;
                    free(cmd);
                    cmd = NULL;
                    closedir(dir);
                    puts("system() failed : Error to run the command");
                    return errno;
                }
                break;
            }
            index++;
        }
    }
    free(subdirPath);
    subdirPath = NULL;
    free(cmd);
    cmd = NULL;
    closedir(dir);
    return 0;
}

int performOperation(char *path)
{
    int ret;
    int op = rand() % 4;
    switch (op) 
    {
        case 0:
            puts("perform op create file");
            ret = createFile(path);
            if(ret != 0)
            {
                puts("createFile() failed in performOperation()");
                return errno;
            }
            break;

        case 1:
            puts("perform op delete file");
            ret = deleteFile(path);
            if(ret != 0)
            {
                puts("deleteFile() failed in performOperation()");
                return errno;
            }
            break;

        case 2:
            puts("perform op create subdir");
            ret = createSubdir(path);
            if(ret != 0)
            {
                puts("createSubdir() failed in performOperation()");
                return errno;
            }
            break;

        case 3:
            puts("perform op delete subdir");
            ret = deleteSubdir(path);
            if(ret != 0)
            {
                puts("deleteSubdir() failed in performOperation()");
                return errno;
            }
            break;

        default:
            puts("in default of perform op");
            break;
    }
    return 0;
}

void randomDepth(char *path, int depth, char *fullPath) 
{
    if (depth == 0) 
    {
        strcpy(fullPath, path);
        return;
    } 
    else 
    {
        DIR *dir;
        struct dirent *entry;
    
        if ((dir = opendir(path)) != NULL) 
        {
            while ((entry = readdir(dir)) != NULL) 
            {
                if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
                {
                    sprintf(fullPath,"%s/%s", path, entry->d_name);
                    randomDepth(fullPath, --depth, fullPath);
                    break;
                }
            }
            closedir(dir);
        }
    }
}