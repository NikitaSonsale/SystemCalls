#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
/*
 *	int stat(const char *pathname, struct stat *statbuf);
 *	return information about a file which is at given pathname, in the buffer pointed to by statbuf.
 *	
 *      int fstat(int fd, struct stat *statbuf);
 *	return information of a file by descriptor, in the buffer pointed to by statbuf.
 *
 *	On success, zero is returned.  On error, -1 is returned,
 *
 *      Commannd line -> stat [options] <FileName>
 */

int main(int argc, char *argv[])
{
	int s;
	if(argc != 2)
	{
		printf("Need at least one argument\n");
		exit(1);
	}

	struct stat sfile;
	s = stat(argv[1], &sfile);
        if(s != 0)
	{
		perror(NULL);
		exit(1);
	}	

	printf("ID of device                : %ld\n", sfile.st_dev);
	printf("Inode Number                : %ld\n", sfile.st_ino);

	printf("permissions                 : ");
	printf((sfile.st_mode & S_IRUSR)? "r":"-");
	printf((sfile.st_mode & S_IWUSR)? "w":"-");
  	printf((sfile.st_mode & S_IXUSR)? "x":"-");
 	printf((sfile.st_mode & S_IRGRP)? "r":"-");
  	printf((sfile.st_mode & S_IWGRP)? "w":"-");
  	printf((sfile.st_mode & S_IXGRP)? "x":"-");
  	printf((sfile.st_mode & S_IROTH)? "r":"-");
  	printf((sfile.st_mode & S_IWOTH)? "w":"-");
  	printf((sfile.st_mode & S_IXOTH)? "x":"-");
  	printf("\n");

	printf("Number of hard links        : %ld\n", sfile.st_nlink);
	printf("User ID of owner            : %d\n" , sfile.st_uid);
	printf("Group ID of owner           : %d\n" , sfile.st_gid);
	printf("Device ID                   : %ld\n", sfile.st_rdev);
	printf("size of file in bytes       : %ld\n", sfile.st_size);
	printf("Block size                  : %ld\n", sfile.st_blksize);
	printf("No of 512B blocks allocated : %ld\n", sfile.st_blocks);
	
	printf("Time of last access         : %s", ctime(&sfile.st_atime));
	printf("Modification time           : %s", ctime(&sfile.st_mtime));
    	printf("Change time                 : %s", ctime(&sfile.st_mtime));


	return 0;
}
