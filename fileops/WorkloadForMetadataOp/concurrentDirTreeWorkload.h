#ifndef __CONCURRENTDIRTREEWORKLOAD__
#define __CONCURRENTDIRTREEWORKLOAD__

#define MAX_PATH      300
#define MAX_NAME      20
#define NO_OF_THREADS 10

struct arguments
{
    char *directoryName;
    int noOfFiles;
    int noOfSubdirectories;
    int depthOfTree;
    int maxFileSize;
};

// function to get arguments from command line
int getArguments(struct arguments *arg, int argc, char *argv[]);

// threads initializations
void *metadataOps(void *arg);

// function to create subdirectories
int createDirectories(char *dirName, int noOfFiles, int noOfSubdir, int depthOfTree, int maxFileSize);

// function to create regular files
int createRegFiles(char *dirName, int noOfFiles, int maxFileSize);

// function to create directory tree
int createDirTree(struct arguments *arg);

// function to write random data in regular files.
int writeFile(char *fileName, int fileSize, int maxFileSize);

// function to create a random reg. file
int createFile(char *path);

// function to delete any random file from subdir
int deleteFile(char *path);

// function to create random subdir
int createSubdir(char *path);

// function to delete any random subdir 
int deleteSubdir(char *path);

// function to perform any random operation at random depth
int performOperation(char *path);

// function to traverse at random depth
void randomDepth(char *path, int depth, char *fullPath);

#endif