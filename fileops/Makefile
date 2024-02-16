CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11

all: chTimeStamp/changeTimeStamp DirectoryTree/dirTree DirTraverse/dirTraversal ReadWriteFile/readWriteFile MultiTApp/concurretOpenDeleteFile WorkloadForMetadataOp/workloadMetadataOps

# chTimeStamp
chTimeStamp/changeTimeStamp: chTimeStamp/chAccessTime.o chTimeStamp/client.o
	$(CC) $(CFLAGS) $^ -o $@

chTimeStamp/client.o: chTimeStamp/client.c
	$(CC) $(CFLAGS) -c $< -o $@

chTimeStamp/chAccessTime.o: chTimeStamp/chAccessTime.c chTimeStamp/chAccessTime.h 
	$(CC) $(CFLAGS) -c $< -o $@

# DirectoryTree application
DirectoryTree/dirTree: DirectoryTree/DirTree.o DirectoryTree/client.o
	$(CC) $(CFLAGS) $^ -o $@

DirectoryTree/client.o: DirectoryTree/client.c
	$(CC) $(CFLAGS) -c $< -o $@

DirectoryTree/DirTree.o: DirectoryTree/DirTree.c DirectoryTree/DirTree.h 
	$(CC) $(CFLAGS) -c $< -o $@

# DirTraverse application
DirTraverse/dirTraversal: DirTraverse/traverseDir.o DirTraverse/client.o
	$(CC) $(CFLAGS) $^ -o $@

DirTraverse/client.o: DirTraverse/client.c
	$(CC) $(CFLAGS) -c $< -o $@

DirTraverse/traverseDir.o: DirTraverse/traverseDir.c DirTraverse/traverseDir.h
	$(CC) $(CFLAGS) -c $< -o $@

# Reading and writing in file
ReadWriteFile/readWriteFile: ReadWriteFile/ReadWrite.o ReadWriteFile/client.o
	$(CC) $(CFLAGS) $^ -o $@

ReadWriteFile/client.o: ReadWriteFile/client.c
	$(CC) $(CFLAGS) -c $< -o $@

ReadWriteFile/ReadWrite.o: ReadWriteFile/ReadWrite.c ReadWriteFile/ReadWrite.h
	$(CC) $(CFLAGS) -c $< -o $@

# multiTApp
MultiTApp/concurretOpenDeleteFile: MultiTApp/openDeleteMul.o MultiTApp/client.o
	$(CC) $(CFLAGS) $^ -o $@

MultiTApp/client.o: MultiTApp/client.c
	$(CC) $(CFLAGS) -c $< -o $@

MultiTApp/openDeleteMul.o: MultiTApp/openDeleteMul.c MultiTApp/openDeleteMul.h
	$(CC) $(CFLAGS) -c $< -o $@

# workloadForMetadataOps
WorkloadForMetadataOp/workloadMetadataOps: WorkloadForMetadataOp/concurrentDirTreeWorkload.o WorkloadForMetadataOp/client.o
	$(CC) $(CFLAGS) $^ -o $@

WorkloadForMetadataOp/client.o: WorkloadForMetadataOp/client.c
	$(CC) $(CFLAGS) -c $< -o $@

WorkloadForMetadataOp/concurrentDirTreeWorkload.o: WorkloadForMetadataOp/concurrentDirTreeWorkload.c WorkloadForMetadataOp/concurrentDirTreeWorkload.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm chTimeStamp/changeTimeStamp DirectoryTree/dirTree DirTraverse/dirTraversal ReadWriteFile/readWriteFile MultiTApp/concurretOpenDeleteFile WorkloadForMetadataOp/workloadMetadataOps
	rm chTimeStamp/client.o DirectoryTree/client.o DirTraverse/client.o ReadWriteFile/client.o MultiTApp/client.o WorkloadForMetadataOp/client.o
	rm chTimeStamp/chAccessTime.o DirectoryTree/DirTree.o DirTraverse/traverseDir.o ReadWriteFile/ReadWrite.o MultiTApp/openDeleteMul.o WorkloadForMetadataOp/concurrentDirTreeWorkload.o