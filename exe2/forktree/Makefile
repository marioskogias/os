.PHONY: all clean

all:  ask2-fork random-tree random-tree-signals random-tree-pipes

CC = gcc
CFLAGS = -g -Wall -O2
SHELL= /bin/bash

random-tree-pipes: random-tree-pipes.o proc-common.o tree.o
	$(CC) $(CFLAGS) $^ -o $@	

random-tree-signals: random-tree-signals.o proc-common.o tree.o
	$(CC) $(CFLAGS) $^ -o $@

random-tree: random-tree.o proc-common.o tree.o
	$(CC) $(CFLAGS) $^ -o $@ 


ask2-fork: ask2-fork.o proc-common.o
	$(CC) $(CFLAGS) $^ -o $@


%.s: %.c
	$(CC) $(CFLAGS) -S -fverbose-asm $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.i: %.c
	gcc -Wall -E $< | indent -kr > $@

clean: 
	rm -f *.o ask2-fork random-tree random-tree-signals random-tree-pipes
