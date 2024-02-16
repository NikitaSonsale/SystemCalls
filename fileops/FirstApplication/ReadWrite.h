#ifndef __READWRITE__
#define __READWRITE__

// structure for command line arguments

struct arguments
{
    char *FileName;
    unsigned long sizeOfFile;
    unsigned long sizeOfChunk;
};

// function to get command line arguments as a input
void getArguments(struct arguments *arg, int argc, char *argv[]);

// function to open file a if exists otherwise to create it.
void creatFile(struct arguments *arg);

// function to write in file upto given file size
void writeFile(struct arguments *arg);

// function to read from file
void readFile(struct arguments *arg);

// To indicate successful creation of file
void printOP(struct arguments *arg);

// function for memory allocation
void *xmalloc(size_t sizeInBytes);

#endif
