#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
/*
 *        ssize_t read(int fd, void *buf, size_t count);
 *        read() read up to count bytes from file descriptor fd into the buffer starting at buf.
 *        
 *        On  success,  the number of bytes read is returned (zero indicates end of file), and 
 *        the file position is advanced by this number. On error, -1 is returned
 */         

int main(int argc, char *argv[])
{
	int fd;
	char buffer[20];

	int sizeOfData = 0;
	if(argc != 2)
	{
		printf("Need at least 2 arguments\n");
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	if(fd == -1)
	{
		perror(NULL);
		exit(1);
	}

	// when sizeOfData becomes 0 then that is end of file so control flow goes out of while loop

	while((sizeOfData = read(fd, buffer, (sizeof(buffer)-1))) > 0)
	{
		if(sizeOfData != 20)
		{
			buffer[sizeOfData] = '\0';
		}
		printf("%s", buffer);
	}

	printf("\n");
	close(fd);
	return 0;
}


