#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <sys/stat.h>
#include "file_ops.h"

int creatFile(char *fileName, mode_t mode)
{
    int fd;

    fd = open(fileName, O_CREAT|O_RDWR, mode);
    if(fd == -1)
    {
        perror("Error to create file");
        return errno;
    }

    return fd;
}

int openFile(char *fileName, int flags)
{
    int fd;
    fd = open(fileName, flags);
    if(fd == -1)
    {
        perror("Error to open the file");
        return errno;
    }

    return fd;
}

int readFile(char *fileName)
{
    int fd;
    int count;
    char *buffer = (char *)malloc(sizeof(char) * SIZE_OF_CHUNK);
    if(buffer == NULL)
    {
        perror("malloc failed : unable to allocate memory to buffer in readFile()");
        return errno;
    }

    fd = openFile(fileName, O_RDONLY);
    if(fd == -1)
    {
        free(buffer);
        buffer = NULL;
        return errno;
    }

    while((count = read(fd, buffer, SIZE_OF_CHUNK)) > 0)
    {
        // do nothing
    }
    
    free(buffer);
    buffer = NULL;
    return 0;
}

int writeFile(char *fileName)
{
    int fd;
    int i;
    int j;
    char *buffer = (char*)malloc(sizeof(char) * SIZE_OF_CHUNK);
    if(buffer == NULL)
    {
        perror("malloc failed : unable to allocate memory to buffer in writeFile()");
        return errno;
    }
    
    fd = openFile(fileName, O_WRONLY);
    if(fd == -1)
    {
        free(buffer);
        buffer = NULL;
        return errno;
    }

    unsigned long ret;
    for(j = 0; j < MAX_FILE_SIZE; j = (j + SIZE_OF_CHUNK))
    {
        for(i = 0; i < SIZE_OF_CHUNK; i++)
        {
            // fill the buffer with random data
            buffer[i] = ('A' + (rand() % 25));
        }

        // writing the buffer into file
		ret = write(fd, buffer, SIZE_OF_CHUNK);
        if (ret < 0)
    	{
			free(buffer);                                                           
            buffer = NULL;
			close(fd);                                                                 
        	printf("unable to write data in file");
        	return -1;
    	}
    }

    free(buffer);
    buffer = NULL;
    close(fd);
    return 0;
}

int chOwner(char *fileName, char* nOwner)
{
    uid_t ownerId;
    
    /*  The getpwnam() function returns a pointer to a structure containing the broken-out 
        fields of the record in the password database 
    */
    ownerId = getpwnam(nOwner)->pw_uid;

    if((chown(fileName, ownerId, -1)) ==  -1)
    {
        perror("chown failed : Error to change the owenership of file in chOwner()\n");
        return errno;
    }

    return 0;
}

int changeMod(char *fileName, mode_t nPermissions)
{

    if((chmod(fileName, nPermissions)) == -1)
    {
        perror("chmod failed : Error to change the permission of file in changeMod\n");
        return errno;
    }

    return 0;
}

