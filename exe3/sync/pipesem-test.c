/*
 * pipesem-test.c
 *
 * A program to verify correct operation of pipesem.
 *
 * Vangelis Koukis <vkoukis@cslab.ece.ntua.gr>
 * Operating Systems
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "pipesem.h"

void child(struct pipesem *sem)
{
	printf("Child: sleeping for five seconds\n");
	sleep(5);
	printf("Child: signaling semaphore\n");
	pipesem_signal(sem);
	exit(0);
}

int main(void)
{
	int status;
	pid_t p;
	struct pipesem sem;

	/* Initialize the semaphore to 0 */
	pipesem_init(&sem, 0);

	/* Create a child */
	p = fork();
	if (p < 0) {
		perror("parent: fork");
		exit(1);
	}
	if (p == 0) {
		child(&sem);
		exit(1);
	}

	/* Father */
	printf("Parent: waiting on semaphore\n");
	pipesem_wait(&sem);
	printf("Parent: signaled, program should terminate.\n");

	wait(&status);
	return 0;
}
