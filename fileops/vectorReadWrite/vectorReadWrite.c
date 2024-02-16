#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/uio.h>
#include "vectorReadWrite.h"

// read first 10 bytes from the file using pread()
int positionedRead(char *fileName)
{
    char *buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    if(buffer == NULL)
    {
        perror("malloc in positionedRead()");
        return errno;
    }

    int fd = open(fileName, O_RDONLY);
    if (fd == -1) 
    {
        perror("open() in positionedRead()");
        return errno;
    } 

    off_t offset = 0;
    ssize_t numRead = pread(fd, buffer, 10, offset);
    if (numRead == -1) 
    {
        close(fd);
        free(buffer);
        buffer = NULL;
        perror("pread() in positionedRead()");
        return errno;
    }
    printf("Read %ld bytes from offset %lld: %s\n", numRead, (long long) offset, buffer);

    close(fd);
    free(buffer);
    buffer = NULL;
    return 0;
}

// write 'hello' at the beginning of file using pwrite()
int positionedWrite(char *fileName)
{
    int fd = open(fileName, O_WRONLY);
    if (fd == -1) 
    {
        perror("open");
        return errno;
    }

    off_t offset = 0;
    ssize_t numWritten = pwrite(fd, "Hello", 5, offset);
    if (numWritten == -1) 
    {
        perror("pwrite");
        close(fd);
        return errno;
    }
    printf("Wrote %ld bytes at offset %lld\n", numWritten, (long long) offset);

    close(fd);
    return 0;
}

// read next 10 bytes from the file using preadv()
int vectorPositionedRead(char *fileName)
{
    /*
        struct iovec {
               void  *iov_base;    // Starting address 
               size_t iov_len;     // Number of bytes to transfer 
        };
    
    */
    char *buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    if(buffer == NULL)
    {
        perror("malloc() failed in vectorPositionedRead()");
        return errno;
    }
    int fd = open(fileName, O_RDONLY);
    if (fd == -1) 
    {
        free(buffer);
        buffer = NULL;
        perror("open() in vectorPositionedRead()");
        return errno;
    } 
    struct iovec iov[2];
    ssize_t numRead;

    off_t offset = 10;
    iov[0].iov_base = buffer;
    iov[0].iov_len = 5;
    iov[1].iov_base = buffer + 5;
    iov[1].iov_len = 5;
    numRead = preadv(fd, iov, 2, offset);
    if (numRead == -1) 
    {
        free(buffer);
        buffer = NULL;
        close(fd);
        perror("preadv() failed in vectorPositionedRead");
        return errno;
    }
    printf("Read %ld bytes from offset %lld: %s\n", numRead, (long long) offset, buffer);

    free(buffer);
    buffer = NULL;
    close(fd);
    return 0;
}

int vectorPositionedWrite(char *fileName)
{
    int fd = open(fileName, O_WRONLY);
    if (fd == -1) 
    {
        perror("open");
        return errno;
    }

    struct iovec iov[1];
    off_t offset = 5;
    iov[0].iov_base = "world";
    iov[0].iov_len = 5;
    ssize_t numWritten = pwritev(fd, iov, 1, offset);
    if (numWritten == -1) 
    {
        close(fd);
        perror("pwritev");
        return errno;
    }
    printf("Wrote %ld bytes at offset %lld\n", numWritten, (long long) offset);

    close(fd);
    return 0;
}
