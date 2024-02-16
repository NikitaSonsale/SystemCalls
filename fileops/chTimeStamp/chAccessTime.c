#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include "chAccessTime.h"

int getArguments(struct cmdArg *arguments, int argc, char *argv[])
{
    int option, i;
    
    if(argc < 2 || argc == 3 || argc == 5 || argc > 7)
    {
        printf("Usage : ./a.out -d <DirPath> -p <newPermissions> -o <ownerName>\n");
        return -1;
    }

    /* -d for name of directory Path
       -p for new permissions
       -o to change the owner to
    */   
    while((option = getopt(argc, argv, "d:p:o:")) != -1)
    {
        switch(option)
        {
            case 'd':
                arguments->dirPath = optarg;  
                break;

            case 'p':
                arguments->newPermission = strtol(optarg, 0, 8);			
                break;
        
            case 'o':
                arguments->ownerName = optarg;
                break;

            default:
                printf("Invalid Flag\n");
        }
    }

    // To handle non option arguments
    for(i = optind; i < argc; i++)
    {
        printf("Non-option argument %s\n", argv[i]);
        printf("Usage : ./a.out -d <DirPath> -p <newPermissions> -o <ownerName>\n");
        return -1;
    }
    return 0;
}

int traverseDir(struct cmdArg *arguments)
{
    int ret;
    ret = iterateDir(arguments->dirPath, arguments->newPermission, arguments->ownerName);
    if(ret != 0)
    {
        perror("Error in iterateDir");
        return errno;
    }
    return 0;
}

int iterateDir(char *pathOfDir, int nPermissions, char* nOwner)
{
    DIR *dir;
    struct dirent *dirEntry;
    char *fullpath = NULL;
    int ret;
   
    fullpath = (char*)malloc(MAX_PATH_SIZE * sizeof(char));
    if(fullpath == NULL)
    {
        printf("malloc failed : unable to allocate memory for path in iterateDir()\n"); 
        return errno;
    }

    dir = opendir(pathOfDir);
    if(dir == NULL)
    {
        free(fullpath);
        fullpath = NULL;
        return errno;
    }

    ret = createRegFiles(pathOfDir); 
    if(ret != 0)
    {
        free(fullpath);
        fullpath = NULL;   
        perror("Error to create no. of regular files in itrateDir()");
        return errno;
    }

    // readdir() returns NULL on reaching the end of the directory stream
    while((dirEntry = readdir(dir)) != NULL) 
    {
        if(dirEntry->d_type == DT_DIR)
        {
            if(strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) 
            {
                continue;
            }

            sprintf(fullpath, "%s/%s", pathOfDir, dirEntry->d_name);

            // traversing directory recursively.
            iterateDir(fullpath, nPermissions, nOwner);
        }

        // if file type is regular file then change the owner, permissions and access and modification time
        else
        {
            sprintf(fullpath, "%s/%s", pathOfDir, dirEntry->d_name);
            ret = changeMod(fullpath, nPermissions);
            if(ret != 0)
            {
                free(fullpath);
                fullpath = NULL;
                perror("Error in changeMod() function");
                return errno;
            }
            
            ret = chOwner(fullpath, nOwner);
            if(ret != 0)
            {
                free(fullpath);
                fullpath = NULL;
                perror("Error in chOwner() function");
                return errno;
            }

            chTimeStamp(fullpath);
            if(ret != 0)
            {
                free(fullpath);
                fullpath = NULL;
                perror("Error in chTimeStamp() function");
                return errno;
            }
        }
    }
    closedir(dir);
    return 0;
}

int createRegFiles(char *dirName)
{
    int i;
    int fd;
    int ret;

    char *regFilePath = (char*)malloc(MAX_PATH_SIZE * sizeof(char));
    if(regFilePath == NULL)
    {
        printf("malloc failed : unable to allocate memory for path in createRegFiles()\n");
        return errno;
    }

    for (i = 0; i < NO_OF_FILES; i++) 
    {
        // generating different names for regular files
        sprintf(regFilePath, "%s/testFile%d.txt", dirName, i);
        ret = creatFile(regFilePath);
        if(ret != 0)
        {
            free(regFilePath);
            regFilePath = NULL;
            perror("Error in createRegFile() to create a file");
            return errno;
        }
    }
    return 0;
}

int chTimeStamp(char *fileName)
{
    struct stat sb;

    if (stat(fileName, &sb) == -1) 
    {
        printf("stat failed : Error to get the metadata info. in chTimeStamp()\n");
        return errno;
    }

    struct timespec times[2];
    times[0] = sb.st_atim;
    times[1] = sb.st_mtim;

    // Add one day to the access time
    times[0].tv_sec += 24 * 60 * 60;
    times[0].tv_nsec += 24 * 60 * 60;

    // Add one day to the modification time
    times[1].tv_sec += 24 * 60 * 60;
    times[1].tv_nsec += 24 * 60 * 60;

    /*
        int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags);

        1)  dirfd    -> File descriptor referring to the directory containing file or dir to be updated.
            AT_FDCWD -> To indicate that the file path is relative to current working directory.

        2)  pathname -> Path of file or dir.

        3)  times    -> array of 2 struct timespec representing the access and modification time

        4)  flags    -> 0 or AT_SYMLINK_NOFOLLOW means that pathname should not be dereferenced 
                        if it is a symbolic link i.e if pathname refers to a symbolic link, the 
                        times of the link itself are updated, rather than the times of the file 
                        it points to. This is the default behavior.  

        On  success, 0 is returned and on error, -1 is returned
       
    */

    if (utimensat(AT_FDCWD, fileName, times, AT_SYMLINK_NOFOLLOW) == -1) 
    {
        printf("utimensat failed : Error to change the access and modification time in chTimeStamp()\n");
        return errno;
    }

    return 0;
}

int creatFile(char *fileName)
{
    int fd;
    fd = open(fileName, O_CREAT|O_RDWR, S_IRWXU);
    if(fd == -1)
    {
        printf("open failed : Error to open file in creatFile()\n");
        return errno;
    }
    close(fd);

    return 0;
}

int changeMod(char *fileName, int nPermissions)
{
    int fd;
    fd = open(fileName, O_RDONLY);
    if(fd == -1)
    {
        printf("open failed : Error to open file in changeMod()\n");
        return errno;
    }

    if((fchmod(fd, nPermissions)) == -1)
    {
        printf("fchmod failed : Error to change the permission of file in changeMod\n");
        return errno;
    }

    close(fd);
    return 0;
}

int chOwner(char *fileName, char* nOwner)
{
    int fd;
    uid_t ownerId;
    fd = open(fileName, O_RDONLY);
    if(fd == -1)
    {
        printf("open failed : Error to open file in chOwner()\n");
        return errno;
    }

    /*  The getpwnam() function returns a pointer to a structure containing the broken-out 
        fields of the record in the password database 
    */
    ownerId = getpwnam(nOwner)->pw_uid;

    /* 
        int fchown(int fd, uid_t owner, gid_t group);
        
        fd    -> file descriptor
        owner -> owner Id (0 - change the owner to root)
        group -> Group Id to be changed (-1 means don't change the group ID)
    */ 
    if((fchown(fd, ownerId, -1)) ==  -1)
    {
        printf("fchown failed : Error to change the owenership of file in chOwner()\n");
        return errno;
    }

    close(fd);
    return 0;
}