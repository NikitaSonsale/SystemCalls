#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void copy(int fdOld, int fdNew);

int main(int argc, char* argv[])
{
	int fdOld, fdNew;
	if(argc != 3)
	{
		printf("Need at least 2 arguments to copy\n");
		exit(1);
	}	

	fdOld = open(argv[1], O_RDONLY);
	if(fdOld == -1)
	{
		perror(NULL);
		exit(1);
	}

	fdNew = creat(argv[2], S_IRWXU);
	if(fdNew == -1)
	{
		perror(NULL);
		exit(1);
	}

	copy(fdOld, fdNew);
	close(fdOld);
	close(fdNew);
	return 0;
}

void copy(int fdOld, int fdNew)
{
	int buffer[20];
	int sizeOfData;
	while((sizeOfData = read(fdOld, buffer, sizeof(buffer))) > 0)
	{
		write(fdNew, buffer, sizeOfData);
	}
}
