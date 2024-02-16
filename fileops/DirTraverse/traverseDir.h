#ifndef __TRAVERSEDIR__
#define __TRAVERSEDIR__

#define MAX_PATH_SIZE 300

// function to get arguments from command line
char* getArguments(int argc, char *argv[]);

// function to traverse directory recursively
void iterateDir(char *pathOfDir);

// function to print metadata of the file
void printMetadata(char *fileName);

#endif