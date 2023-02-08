#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

void copy(int fdOld, int fdNew);

int main(int argc, char* argv[])
{
	int option;
	int fdOld, fdNew;
	int i;
	
	if(argc < 2 || argc == 3 || argc > 5)
	{
		printf("Usage : ./a.out -a <OldFile> -b <NewFile>\n");
		exit(1);
	}

	//getopt() returns -1 if there are no more options to process.
	while((option = getopt(argc, argv, "a:b:")) != -1)
	{
		switch(option)
		{
			case 'a':
				fdOld = open(optarg, O_RDONLY);
			        if(fdOld == -1)
        			{
                			perror(NULL);
                			exit(1);
        			}
				break;

			case 'b':

				fdNew = creat(optarg, S_IRWXU);
			        if(fdNew == -1)
        			{
                			perror(NULL);
               				exit(1);
        			}
				break; 

			default:
				exit(1);
		}
	}
	
	for (i = optind; i < argc; i++)
	{
		printf ("Non-option argument %s\n", argv[i]);
		printf("Usage : ./a.out -a <OldFile> -b <NewFile>\n");
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
