#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <getopt.h>

/*
 *	int chmod(const char *pathname, mode_t mode);
 *	the chmod command is used to change the access mode of a file.
 *
 *	On success, zero is returned.  On error, -1 is returned
 */

int main(int argc, char *argv[])
{
	int option;
	int i;
	int j;
	char *FileName;
	if(argc < 2 || argc == 3 || argc > 5)
	{
		printf("Usage : ./a.out -f <FileName> -p <NewPermissions>\n");
		exit(0);
	}

	while((option = getopt(argc, argv, "f:p:")) != -1)
	{
		switch(option)
		{
			case 'f':
				FileName = optarg;		
				break;
			case 'p':
				i = strtol(optarg, 0, 8);			
				break;
			default:
				exit(1);
		}
	}

	for(j = optind; j < argc; j++)
	{
		printf("Non-option argument %s\n", argv[j]);
		printf("Usage : ./a.out -f <FileName> -p <NewPermissions>\n");
		exit(1);
	}

	// change the mode of the file
	if((chmod(FileName, i)) < 0)
	{
		perror(NULL);
		exit(0);
	}

	return 0;
}
