#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/*
 *    off_t lseek(int fd, off_t offset, int whence);
 *    Upon successful completion, lseek() returns the resulting offset location as measured in 
 *    bytes from the beginning of the file.  On error, the value (off_t) -1 is returned
 */

char buffer[5];
int main(int argc, char *argv[])
{
	int fd;
	off_t offset;
	int size;
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

	while((size = read(fd, buffer, sizeof(buffer))) > 0)
	{
		if(size != 5)
		{
			buffer[size] = '\0';
		}
		printf("%s", buffer);
		offset = lseek(fd, 3, SEEK_CUR);
	}
	close(fd);

	return 0;
}
