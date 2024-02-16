#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <aio.h>
#include <errno.h>

#define MAX_BUF_SIZE 512

int readFile(char *fileName);

int main(int argc, char *argv[]) 
{
    int ret;
    if (argc != 2) 
    {
        printf("Usage: ./a.out <filename>\n");
        return EXIT_FAILURE;
    }

    ret = readFile(argv[1]);
    if(ret != 0)
    {
        puts("readFile failed");
        return EXIT_FAILURE;
    }
    return 0;
}

int readFile(char *fileName)
{
    int fd, ret;
    struct aiocb aio;
    /*
        struct aiocb {
               // The order of these fields is implementation-dependent 

               int             aio_fildes;     // File descriptor 
               off_t           aio_offset;     // File offset 
               volatile void  *aio_buf;        // Location of buffer 
               size_t          aio_nbytes;     // Length of transfer 
               int             aio_reqprio;    // Request priority 
               struct sigevent aio_sigevent;   // Notification method 
               int             aio_lio_opcode; // Operation to be performed;
                                                  lio_listio() only 

               // Various implementation-internal fields not shown 
           };

    */

    off_t offset = 0;
    int total_bytes_read = 0;

    char *buffer = (char*)malloc(sizeof(char) * MAX_BUF_SIZE);
    if(buffer == NULL)
    {
        perror("malloc failed : unable to allocate memory to buffer");
        return errno;
    }

    fd = open(fileName, O_RDONLY);
    if (fd < 0) 
    {
        perror("open");
        return errno;
    }

    /* initialize the aio control block */
    memset(&aio, 0, sizeof(struct aiocb));
    aio.aio_fildes = fd;
    aio.aio_buf = buffer;
    aio.aio_nbytes = MAX_BUF_SIZE;

    /* initiate the asynchronous read operation */
    while(1)
    {
        aio.aio_offset = offset;

        /* The aio_read() function queues the I/O request.
           The "asynchronous" means that this call returns 
           as soon as the request has been enqueued; 
           the read may or may not have completed when the call returns.
           On success, 0 is returned..  
           On error, the request is not enqueued, -1 is returned
        */
        ret = aio_read(&aio);
        if (ret < 0) 
        {
            close(fd);
            free(buffer);
            buffer = NULL;
            perror("aio_read");
            return errno;
        }

        /* wait for the operation to complete 
           int aio_error(const struct aiocb *aiocbp);
           The  aio_error() function returns the error status for the asynchronous I/O request
           EINPROGRESS, if the request has not been completed yet.
           return 0, if the request completed successfully.
        */

        while (aio_error(&aio) == EINPROGRESS) 
        {
            printf("waiting for read to complete.\n");
        }
        

        /* checking for errors */
        ret = aio_error(&aio);
        if (ret != 0) 
        {
            close(fd);
            free(buffer);
            buffer = NULL;
            perror("aio_error");
            return errno;;
        }

        /*
           The  aio_return() function returns the final return status 
           for the asynchronous I/O request.
           This  function should be called only once for any given request, 
           after aio_error() returns something other than EINPROGRESS.
           On  success,  the  number of bytes read is returned
           On error, -1 is returned.
        */
        ret = aio_return(&aio);
        if (ret < 0) 
        {
            close(fd);
            free(buffer);
            buffer = NULL;
            perror("aio_return");
            return errno;
        }

        total_bytes_read += ret;
        offset += ret;

        if(ret == 0)
        {
            break;
        }

        printf("read %d bytes from file:\n%s\n", ret, buffer);
    }

    close(fd);
    free(buffer);
    buffer = NULL;
    return 0;
}