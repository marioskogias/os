#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "tree.h"
#include "proc-common.h"

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
	exit(1);
}



void createTree(struct tree_node * root) {
	printf("process %s started\n",root->name);	
	if (root->children == NULL)
		fork_procs(root->name,5);
	else {
		int i;	
		pid_t pid;
		int status;
		change_pname(root->name);
		for (i=0;i<root->nr_children;i++) {
			pid = fork();
			if (pid == -1) {
				perror("createTree: fork");
				exit(1);
			}
			if (pid == 0) {
				createTree(root->children+i);
				exit(1);
			}
		}
		for (i=0;i<root->nr_children;i++) {	 // get the status		
			pid = wait(&status);

			explain_wait_status(pid,status);				
		}
	} 
		
	printf("%s: Exiting...\n",root->name);
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
		
	
	
	sleep(3);	
	
	show_pstree(pid);
	
	pid = wait(&status);
	explain_wait_status(pid, status);


	return 0;


}
