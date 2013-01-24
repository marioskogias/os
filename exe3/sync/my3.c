/*
 * procs-shm.c
 *
 * A program to create three processes,
 * working with a shared memory area.
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
#include "proc-common.h"
#include "pipesem.h"

/*
 * This is a pointer to a shared memory area.
 * It holds an integer value, that is manipulated
 * concurrently by all children processes.
 */

int *shared_memory;
/* ... */
struct pipesem sem1;
struct pipesem mutexAC;
struct pipesem mutexAB;
struct pipesem mutexBC;

/* Proc A: n = n + 1 */

void proc_A(void)
{
	volatile int *n = &shared_memory[0];
        int i;
	/* ... */
           pipesem_signal(&sem1);
           pipesem_signal(&sem1);
        /* ... */

        for (;;) {
 		/* ... */
//                pipesem_wait(&mutexAC);
               // pipesem_wait(&mutexAC);
                pipesem_wait(&sem1);

		/* ... */
                  printf("A ");
		*n = *n + 1;

		/* ... */
                if (*n==3) {
                   pipesem_signal(&mutexAB);	
                   continue;	           
        	}  // else pipesem_signal(&mutexAC) ;                  
                /* ... */

	}

	exit(0);
}

/* Proc B: n = n - 2 */
void proc_B(void)
{
	volatile int *n = &shared_memory[0];
	/* ... */


	for (;;) {

                /* ... */
 		   // pipesem_wait(&mutexAB);
                    pipesem_wait(&mutexAB);   
         	/* ... */

		*n = *n - 2;

		/* ... */
                 if(*n==1) {
                   pipesem_signal(&mutexBC);
                   continue;
                 } 
		/* ... */

	}
	exit(0);
}

/* Proc C: print n */
void proc_C(void)
{
	int val,i;
	volatile int *n = &shared_memory[0];

	for (;;) {
  		/* ... */
                pipesem_wait(&mutexBC);
		/* ... */
		val = *n;
		printf("Proc C: n = %d\n", val);
		if (val != 1) {
			printf("     ...Aaaaaargh!\n");
        	}

 		/* ... */
                 for(i=0; i<2; i++) 
 	 	   pipesem_signal(&sem1);
  //               pipesem_signal(&mutexAC);
    //             pipesem_signal(&mutexAC);
         //        pipesem_signal(&mutexAC); 
		/* ... */

	}
	exit(0);
}

/*
 * Use a NULL-terminated array of pointers to functions.
 * Each child process gets to call a different pointer.
 */
typedef void proc_fn_t(void);
static proc_fn_t *proc_funcs[] = {proc_A, proc_B, proc_C, NULL};

int main(void)
{
	int i;
	int status;
	pid_t p;
	proc_fn_t *proc_fn;
        pipesem_init(&sem1, 0);
        pipesem_init(&mutexAC,1);
        pipesem_init(&mutexAB,0);
        pipesem_init(&mutexBC,0);
	/* ... */


	/* Create a shared memory area */
	shared_memory = create_shared_memory_area(sizeof(int));
	*shared_memory = 1;

	for(i = 0; (proc_fn = proc_funcs[i]) != NULL; i++) {
		printf("%lu fork()\n", (unsigned long)getpid());
		p = fork();
		if (p < 0) {
			perror("parent: fork");
			exit(1);
		}
		if (p != 0)  { 
                	/* Father */
			continue;
		}
		/* Child */
		proc_fn();
		assert(0);
	}

	/* Parent waits for all children to terminate */
	for (; i >0; i--)
		wait(&status);

	return 0;
}

