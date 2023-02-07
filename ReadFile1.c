#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char buffer[20];

int main(int argc, char *argv[])
{
	int n = 10;
	if(argc != 2)
	{
		printf("Need at least 2 arguments\n");
		exit(0);
	}
	int fd;
	fd = open(argv[1], O_RDONLY);
	if(fd == -1)
	{
		perror(NULL);
		exit(1);
	}
	int size;
	while((size = read(fd, buffer, sizeof(buffer))) > 0)
	{
		if(size != 20)
		{
			buffer[size] = '\0';
		}
		printf("%s", buffer);
	}
	return 0;
}
