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

int compute(char * action,int * pipe) {
	int val1,val2;
	
        printf("Child: My PID is %ld. Receiving two ints from the parent.\n",
                (long)getpid());
        if (read(pipe[0], &val1, sizeof(val1)) != sizeof(val1)) {
                perror("child: read from pipe");
                exit(1);
        }
	if (read(pipe[0], &val2, sizeof(val2)) != sizeof(val2)) {
                perror("child: read from pipe");
                exit(1);
        }
        printf("Child: received values %d %d from the pipe. Will now compute.\n", val1,val2);
	
	int res;

	if (strcmp(action,"*")== 0) {
		res = val1*val2;
		printf("pollaplasiasmos\n");
	} else if (strcmp(action,"+")==0) {
		printf("prosthesh\n");
		res = val1+val2;
	}
	
	printf("from compute the result is %d\n",res);
	
	
	if (write(pipe[1], &res, sizeof(res)) != sizeof(res)) {
                perror("parent: write to pipe");
                exit(1);
        }
	
	printf("wrote to pipe %d\n", res);	
        return res;


}

void fork_procs(struct tree_node * root,int * pipe)
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
	int val = atoi(root->name);

	raise(SIGSTOP);
        printf("PID = %ld, name = %s is awake\n",
                (long)getpid(), root->name);

	if (write(pipe[1], &val, sizeof(val)) != sizeof(val)) {
      		perror("leaf: write to pipe");
                exit(1);
        }

	printf("wrote to pipe %d\n",val);
//	sleep(2);

        /* ... */

        /*
         * Exit
         */
        exit(0);

}


struct childPids {

	pid_t pid;
	int childNo;
};

void createTree(struct tree_node * root,pid_t *pid,int * status,int * pipe) {
	
	if (root->children == NULL)
		fork_procs(root,pipe);
	else {
		int i;
                struct childPids  *children = malloc(2*sizeof(struct childPids));
              
		printf("PID = %ld, name %s, starting...\n",
                        (long)getpid(), root->name);
                change_pname(root->name);
                for (i=0;i<2;i++) {
                        *pid = fork();
                        if (*pid == -1) {
                                perror("createTree: fork");
                                exit(1);
                        }
                        if (*pid == 0) {
				free(children-i);
				createTree(root->children+i,pid,status,pipe);
                                exit(0);
                        }
                        children->pid = *pid;
			children->childNo = (root->children+i)->nr_children;
                        children++;

                }
                wait_for_ready_children(root->nr_children);
                printf("Process %s is stopped\n",root->name);
                raise(SIGSTOP);
                printf("PID = %ld, name = %s is awake\n",
                 (long)getpid(), root->name);

		children = children-2;
		pid_t pid1,pid2;
		pid1 = children[0].pid;
		pid2 = children[1].pid;
		if (children[0].childNo == 0) {
			pid1 = children[1].pid;
			pid2 = children[0].pid;
		}
                
		kill(pid1,SIGCONT);    
                wait(status);
                explain_wait_status(pid1,*status);
              
		kill(pid2,SIGCONT);    
                wait(status);
                explain_wait_status(pid2,*status);
  
               
                free(children);
		compute(root->name,pipe);
		exit(1);
		
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

	int mypipe[2];
	 if (pipe(mypipe) < 0) {
                perror("pipe");
                exit(1);
        }
	
	
	


	pid = fork();
	


	if (pid == -1) {
		perror("createTree: fork");
		exit(1);
	}
	if (pid == 0) {
		createTree(root,&pid,&status,mypipe);
//		compute(root->name,mypipe);
		exit(1);
	}
		
	


	wait_for_ready_children(1);
        show_pstree(pid);
        kill(pid,SIGCONT);
        pid = wait(&status);
        explain_wait_status(pid, status);	
	
	
	printf("wait to read\n");	
	int result;
	if (read(mypipe[0], &result, sizeof(result)) != sizeof(result)) {
                perror("child: read from pipe");
                exit(1);
        }	
	
	printf("The final result is %d\n",result);
	return 0;


}
