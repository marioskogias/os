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
        printf("PID = %ld, name = %s is awake\n",
                (long)getpid(), root->name);

        /* ... */

        /*
         * Exit
         */
        exit(0);

}



void createTree(struct tree_node * root,pid_t *pid,int * status) {
	
	if (root->children == NULL)
		fork_procs(root);
	else {
		int i;
		printf("PID = %ld, name %s, starting...\n",
                        (long)getpid(), root->name);
		change_pname(root->name);
		for (i=0;i<root->nr_children;i++) {
			*pid = fork();
			if (*pid == -1) {
				perror("createTree: fork");
				exit(1);
			}
			if (*pid == 0) {
				createTree(root->children+i,pid,status);
				exit(1);
			}
		}
/*		for (i=0;i<root->nr_children;i++) {	 // get the status		
			*pid = wait(status);
			explain_wait_status(*pid,*status);				
		}*/
		wait_for_ready_children(root->nr_children);
		raise(SIGSTOP);
		printf("PID = %ld, name = %s is awake\n",
                (long)getpid(), root->name);

	} 
		
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
		createTree(root,&pid,&status);
		exit(1);		
	}
		
	
	
	
	wait_for_ready_children(1);
	show_pstree(pid);
	
	kill(pid,SIGCONT);
	pid = wait(&status);
	explain_wait_status(pid, status);


	return 0;


}
