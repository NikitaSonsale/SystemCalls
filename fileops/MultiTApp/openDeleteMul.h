#ifndef __OPENDELMUL__
#define __OPENDELMUL__

#define THREAD_RUNTIME_SECONDS 120

char* getArguments(int argc, char *argv[]);
void *read_write_data(void *arg);
void *unlink_and_stat_file(void *arg);
int writeFile(int fd);
int readFile(int fd);

#endif
