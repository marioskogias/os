#
# Makefile
# 

CC = gcc
CFLAGS = -Wall -O2

all: mandel procs-shm pipesem.o pipesem-test

proc-common.o: proc-common.h proc-common.h
	$(CC) $(CFLAGS) -c -o proc-common.o proc-common.c

pipesem.o: pipesem.c pipesem.h
	$(CC) $(CFLAGS) -c -o pipesem.o pipesem.c

## Pipesem
pipesem-test.o: pipesem.h pipesem-test.c
	$(CC) $(CFLAGS) -c -o pipesem-test.o pipesem-test.c

pipesem-test: pipesem.o pipesem-test.o
	$(CC) $(CFLAGS) -o pipesem-test pipesem.o pipesem-test.o

## Mandel
mandel-lib.o: mandel-lib.h mandel-lib.c
	$(CC) $(CFLAGS) -c -o mandel-lib.o mandel-lib.c

mandel.o: mandel-lib.h mandel.c
	$(CC) $(CFLAGS) -c -o mandel.o mandel.c

mandel: mandel-lib.o mandel.o
	$(CC) $(CFLAGS) -o mandel mandel-lib.o mandel.o

## Procs-shm
procs-shm.o: proc-common.h procs-shm.c
	$(CC) $(CFLAGS) -c -o procs-shm.o procs-shm.c

procs-shm: proc-common.o procs-shm.o
	$(CC) $(CFLAGS) -o procs-shm proc-common.o procs-shm.o

clean:
	rm -f *.o pipesem-test mandel procs-shm
