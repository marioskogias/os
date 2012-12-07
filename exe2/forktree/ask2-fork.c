#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "proc-common.h"

#define SLEEP_PROC_SEC  10
#define SLEEP_TREE_SEC  3

/*
 * Create this process tree:
 * A-+-B---D
 *   `-C
 */
void fork_procs(char * name,int sleep_time)
{
	/*
	 * initial process is A.
	 */

	change_pname(name);
	printf("%s: Sleeping...\n",name);
	sleep(sleep_time);

	/* ... */

	printf("%s: Exiting...\n",name);
	if (strcmp(name,"D") == 0)
		exit(13); 
	else if (strcmp(name,"C") == 0)
		exit(17);
}

/*
 * The initial process forks the root of the process tree,
 * waits for the process tree to be completely created,
 * then takes a photo of it using show_pstree().
 *
 * How to wait for the process tree to be ready?
 * In ask2-{fork, tree}:
 *      wait for a few seconds, hope for the best.
 * In ask2-signals:
 *      use wait_for_ready_children() to wait until
 *      the first process raises SIGSTOP.
 */
int main(void)
{
	pid_t pid;
	int status;

	/* Fork root of process tree */
	pid = fork();
	
	if (pid < 0) {
		perror("main: fork");
		exit(1);
	}
	if (pid == 0) {
		/* Child A*/
		printf("Process A started from main\n");
		pid = fork();
		if (pid < 0) {
			perror("main: fork");
			exit(1);
		}
		if (pid == 0) {
			/* Child B*/
			//fork_procs("B",5);
			printf("Process A started process B\n");
			pid = fork();
			if (pid < 0) {
				perror("main: fork");
				exit(1);
			}
			if (pid == 0) {
				printf("Process B started process D\n");
				fork_procs("D",5);
				exit(1);
			}
			change_pname("B");
			//printf("Process B is waiting D for completion\n");
			pid = wait(&status);
			explain_wait_status(pid, status);

			exit(19);
		}
		
		pid = fork();
		if (pid < 0) {
			perror("main: fork");
			exit(1);
		}
		if (pid == 0) {
		/* Child C*/
			printf("Process A started process C\n");
			fork_procs("C",5);
			exit(1);
		}	
		//fork_procs("A",2);
		
		change_pname("A");
		//printf("Process A is waiting B and C for completion\n");
		pid = wait(&status);
		explain_wait_status(pid, status);
		pid = wait(&status);
		explain_wait_status(pid, status);
		
		
		exit(16);
	}
	

	
	/*
	 * Father
	 */
	/* for ask2-signals */
	/* wait_for_ready_children(1); */

	/* for ask2-{fork, tree} */
	sleep(SLEEP_TREE_SEC);

	/* Print the process tree root at pid */
	
	
	show_pstree(pid);
		
	/* for ask2-signals */
	/* kill(pid, SIGCONT); */

	/* Wait for the root of the process tree to terminate */
	//printf("Main is waiting A for completion\n");	
	pid = wait(&status);
	explain_wait_status(pid, status);
	
	return 0;
}
