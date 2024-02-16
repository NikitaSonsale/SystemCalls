CC=gcc
CFLAGS=-Wall -Werror

all: client

file_ops.o: file_ops.c file_ops.h
	$(CC) $(CFLAGS) -c file_ops.c -fPIC

libfileops.so: file_ops.o
	$(CC) $(CFLAGS) -shared -o libfileops.so file_ops.o

client: client.c file_ops.h libfileops.so
	$(CC) $(CFLAGS) -o client client.c -L. -lfileops

clean:
	rm -f *.o *.so client
