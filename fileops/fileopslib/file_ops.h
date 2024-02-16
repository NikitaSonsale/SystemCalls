#ifndef __FILEOPS__
#define __FILEOPS__

#define SIZE_OF_CHUNK 1024
#define MAX_FILE_SIZE 1048576

int creatFile(char *fileName, mode_t mode);
int openFile(char *fileName, int flags);
int readFile(char *fileName);
int writeFile(char *fileName);
int chOwner(char *fileName, char* nOwner);
int changeMod(char *fileName, mode_t nPermissions);

#endif