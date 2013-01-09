#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#include "tree.h"
#include "proc-common.h"

void fork_procs(struct tree_node * root)
{
	 /*
         * Start
         */
        printf("PID = %ld, name %s, starting...\n",
                        (long)getpid(), root->name);
        change_pname(root->name);

        /* ... */

        /*
         * Suspend Self
         */
        raise(SIGSTOP);
//	sleep(2);
        printf("PID = %ld, name = %s is awake\n",
                (long)getpid(), root->name);

        /* ... */

        /*
         * Exit
         */
        exit(0);

}



void createTree(struct tree_node * root) {
	
	if (root->children == NULL)
		fork_procs(root);
	else {
		int i;	
		pid_t pid;
		int status;
		pid_t * pids = malloc(root->nr_children*(sizeof(pid_t)));
		printf("PID = %ld, name %s, starting...\n",
                        (long)getpid(), root->name);
		change_pname(root->name);
		for (i=0;i<root->nr_children;i++) {
			pid = fork();
			if (pid == -1) {
				perror("createTree: fork");
				exit(1);
			}
			if (pid == 0) {
				free(pids-i);
				createTree(root->children+i);
				exit(0);
			}
			*pids = pid;
			pids++;	
		}
		wait_for_ready_children(root->nr_children);
		printf("Process %s is stopped\n",root->name);
		raise(SIGSTOP);
		printf("PID = %ld, name = %s is awake\n",
              	 (long)getpid(), root->name);
		pids= pids - root->nr_children;
		for (i=0;i<root->nr_children;i++) {
			kill(*pids,SIGCONT);	
			wait(&status);
			explain_wait_status(*pids,status);
			pids++;
		}
		pids = pids - root->nr_children;
		free(pids);

		}
		exit(0);
} 
		

int main(int argc, char *argv[]) {

	pid_t pid;
	int status;
	struct tree_node * root;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_tree_file>\n\n", argv[0]);
		exit(1);
	}

	root = get_tree_from_file(argv[1]);
	pid = fork();
	


	if (pid == -1) {
		perror("createTree: fork");
		exit(1);
	}
	if (pid == 0) {
		createTree(root);
		exit(1);		
	}
		
	
	
	
	wait_for_ready_children(1);
	show_pstree(pid);
	kill(pid,SIGCONT);
	pid = wait(&status);
	explain_wait_status(pid, status);


	return 0;


}
