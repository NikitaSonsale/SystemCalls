#include <stdio.h>           // declaration of puts() and perror()
#include <fcntl.h>           // declaration of open()
#include <unistd.h>          // declaration of close()

/*
 int open(const char *pathname, int flags);
 int open(const char *pathname, int flags, mode_t mode);
 int creat(const char *pathname, mode_t mode);

 The 3rd argument i.e mode must be supplied when O_CREAT or 
 O_TMPFILE(Create an unnamed temporary regular file) is 
 specified in flags. If neither O_CREAT nor O_TMPFILE is 
 specified, then mode is ignored.

 open() and creat() return the new file descriptor, or -1 if an error occurred 
*/

int main()
{
	int fd;
	fd = open("test.txt", O_RDWR);
	if(fd == -1)
	{
		/* if test.txt does not exist open will return -1 and
		   perror() function prints an error message to stderr
		   No such file or directory.
		*/
		perror(NULL);
	}
	close(fd);

	// if file is not present then create it
	// fd = creat("test.txt", S_IRWXU);
	fd = open("test.txt", O_CREAT|O_RDWR, S_IRWXU);
	if(fd == -1)
	
		perror(NULL);
	else
		puts("file created successfully");
	
	close(fd);
	return 0;
}

