#ifndef __DIRTREE__
#define __DIRTREE__

struct arguments
{
    char *directoryName;
    int noOfFiles;
    int noOfSubdirectories;
    int depthOfTree;
    int maxFileSize;
};

void getArguments(struct arguments *arg, int argc, char *argv[]);
void createDirectories(char *dirName, int noOfFiles, int noOfSubdir, int depthOfTree, int maxFileSize);
void createRegFiles(char *dirName, int noOfFiles, int maxFileSize);
void createDirTree(struct arguments *arg);
void writeFile(char *fileName, int fileSize, int maxFileSize);

#endif