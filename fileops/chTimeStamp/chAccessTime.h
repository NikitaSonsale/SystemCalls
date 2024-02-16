#ifndef __CHACCESSTIME__
#define __CHACCESSTIME__

#define MAX_PATH_SIZE 300
#define NO_OF_FILES   6
#define BUFFER_SIZE   2048

struct cmdArg
{
    char *dirPath;
    int newPermission;
    char* ownerName;
};

// function to get arguments from command line
int getArguments(struct cmdArg *arguments, int argc, char *argv[]);

// wrapper function for iterateDir()
int traverseDir(struct cmdArg *arguments);

// function to iterate the directory
int iterateDir(char *pathOfDir, int nPermissions, char* nOwner);

// function to create no. of regular files
int createRegFiles(char *dirName);

// function to change the access and modification time of file
int chTimeStamp(char *fileName);

// function to create a file if not exist
int creatFile(char *fileName);

// function to change the permissions of file
int changeMod(char *fileName, int nPermission);

// function to change the owner and group ID
int chOwner(char *fileName, char* nOwner);

#endif