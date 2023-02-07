#include <stdio.h>          // declaration of perror() and printf()
#include <stdlib.h>         // declaration of exit()
#include <fcntl.h>          // declaration of open()
#include <unistd.h>         // declaration of write() 

/*
	ssize_t write(int fd, const void *buf, size_t count);
	write() writes up to count bytes from the buffer starting at buf to the file referred to by the file descriptor fd.

	On success, the number of bytes written is returned.  On error, -1 is returned
*/

int main(int argc, char *argv[])
{
	int fd;
	char buffer[100] = "In UNIX everything storable is file and internally every file is identified by its unique inode";
	int size = 0;
	if(argc != 2)
	{
		printf("Need at least 1 argument\n");
		exit(1);
	}

	fd = open(argv[1], O_WRONLY);
	if(fd == -1)
	{
		perror(NULL);
		exit(1);
	}
	else
		puts("file opened sucessfully");

	size = write(fd, buffer, 90);
	if(size == -1)
	{
		perror(NULL);
	}
	printf("size : %d\n", size);
	close(fd);
	return 0;
}
