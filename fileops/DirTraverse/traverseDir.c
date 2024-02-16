#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "traverseDir.h"

// function to get arguments from command line
char* getArguments(int argc, char *argv[])
{
    int option, i;
    char *dirPath;
    
    if(argc < 2 || argc > 3)
    {
        printf("Usage : ./a.out -n <DirectoryName>\n");
        exit(1);
    }

    //-n for name of main directory
       
    while((option = getopt(argc, argv, "n:")) != -1)
    {
        switch(option)
        {
            case 'n':
                dirPath = optarg;  
                break;
        
            default:
                exit(1);
        }
    }

    // To handle non option arguments
    for(i = optind; i < argc; i++)
    {
        printf("Non-option argument %s\n", argv[i]);
        printf("Usage : ./a.out -n <DirectoryName>\n");
        exit(1);
    }

    return dirPath;
}

// function to print metadata of the file   
void printMetadata(char *fileName)
{
    struct stat fileMetadata;
    if(stat(fileName, &fileMetadata) == -1) 
    {
        perror("stat");
        return;
    }
    
    printf("ID of device                : %ld\n", fileMetadata.st_dev);
	printf("Inode Number                : %ld\n", fileMetadata.st_ino);

	printf("permissions                 : ");
	printf((fileMetadata.st_mode & S_IRUSR)? "r":"-");
	printf((fileMetadata.st_mode & S_IWUSR)? "w":"-");
  	printf((fileMetadata.st_mode & S_IXUSR)? "x":"-");
 	printf((fileMetadata.st_mode & S_IRGRP)? "r":"-");
  	printf((fileMetadata.st_mode & S_IWGRP)? "w":"-");
  	printf((fileMetadata.st_mode & S_IXGRP)? "x":"-");
  	printf((fileMetadata.st_mode & S_IROTH)? "r":"-");
  	printf((fileMetadata.st_mode & S_IWOTH)? "w":"-");
  	printf((fileMetadata.st_mode & S_IXOTH)? "x":"-");
  	printf("\n");

	printf("Number of hard links        : %ld\n", fileMetadata.st_nlink);
	printf("User ID of owner            : %d\n" , fileMetadata.st_uid);
	printf("Group ID of owner           : %d\n" , fileMetadata.st_gid);
	printf("Device ID                   : %ld\n", fileMetadata.st_rdev);
	printf("size of file in bytes       : %ld\n", fileMetadata.st_size);
	printf("Block size                  : %ld\n", fileMetadata.st_blksize);
	printf("No of 512B blocks allocated : %ld\n", fileMetadata.st_blocks);
	
	printf("Time of last access         : %s", ctime(&fileMetadata.st_atime));
	printf("Modification time           : %s", ctime(&fileMetadata.st_mtime));
    printf("Change time                 : %s", ctime(&fileMetadata.st_mtime));

    printf("\n......................................\n\n");
    
}

// function to traverse directory recursively
void iterateDir(char *pathOfDir)
{
    DIR *dir;
    struct dirent *dirEntry;
    struct stat sb;
    char *fullpath = NULL;

    fullpath = (char*)malloc(MAX_PATH_SIZE * sizeof(char));
    if(fullpath == NULL)
    {
        printf("failed to allocate memory to store path\n");
        exit(1);
    }

    printf("path of directory : %s\n", pathOfDir);
    dir = opendir(pathOfDir);
    if(dir == NULL)
    {
        perror("opedir");
        exit(1);
    }

    // readdir() returns NULL on reaching the end of the directory stream
    while((dirEntry = readdir(dir)) != NULL) 
    {
        if(stat(dirEntry->d_name, &sb) == 0 && S_ISDIR(sb.st_mode))
        {
            if(strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) 
            {
                continue;
            }
            sprintf(fullpath, "%s/%s", pathOfDir, dirEntry->d_name);
            // traversing directory recursively.
            iterateDir(fullpath);
        }

        // if file type is regular file then print metadata
        else if(stat(dirEntry->d_name, &sb) == 0 && S_ISREG(sb.st_mode))
        {
            sprintf(fullpath, "%s/%s", pathOfDir, dirEntry->d_name);
            printf("Name of file : %s\n", dirEntry->d_name);
            printf("Path of file : %s\n", fullpath);
            printMetadata(fullpath);
        }
    }
    closedir(dir);
}

