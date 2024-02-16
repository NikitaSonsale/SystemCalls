#ifndef __VECTORREADWRITE__
#define __VECTORREADWRITE__

#define BUFFER_SIZE 1024

int positionedRead(char *fileName);
int positionedWrite(char *fileName);
int vectorPositionedRead(char *fileName);
int vectorPositionedWrite(char *fileName);

#endif