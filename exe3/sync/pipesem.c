/*
 * pipesem.c
 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
/* The functions are intentionally left blank :-) */

#include "pipesem.h"

void pipesem_init(struct pipesem *sem, int val)
{
	int temp[2];
	if (pipe(temp) < 0) {
                perror("pipe");
		exit(1);
        }
	sem->rfd = temp[0];
	sem->wfd = temp[1];
	
	int * cookies = malloc(val*sizeof(int));
	int i;
	for (i=0;i<val;i++) 
		*(i+cookies) = 1;
	 if (write(sem->wfd, cookies, val*sizeof(int)) != (val*sizeof(int))) {
                perror("parent: write to pipe");
                exit(1);
        }
	//assert( 0 && "I am empty! please fill me!");
}

void pipesem_wait(struct pipesem *sem)
{
	int val;
	if (read(sem->rfd, &val, sizeof(val)) != sizeof(val)) {
                perror("child: read from pipe");
                exit(1);
        }
	//assert( 0 && "I am empty! please fill me!");
}

void pipesem_signal(struct pipesem *sem)
{
	int  val = 1 ;
	if (write(sem->wfd, &val, sizeof(val)) != sizeof(val)) {
		perror("parent: write to pipe");
		exit(1);
	}
	//assert( 0 && "I am empty! please fill me!");
}

void pipesem_destroy(struct pipesem *sem)
{
	assert( 0 && "I am empty! please fill me!");
}
