#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/prctl.h>



void show_pstree(pid_t p) // gia na parw ta pids
{
        int ret;
        char cmd[1024];

        snprintf(cmd, sizeof(cmd), "echo; echo; pstree -G -c -p %ld; echo; echo",
                (long)p);
        cmd[sizeof(cmd)-1] = '\0';
        ret = system(cmd);
        if (ret < 0) {
                perror("system");
                exit(104);
        }
}



int main() {


	pid_t pid;
	int status;
	pid_t pids[2];


	pid = fork();

	if (pid < 0) {
		perror("main: fork");
		exit(1);
	}

	if (pid == 0) {

		prctl(PR_SET_NAME, "root");
		pid = fork();

		if (pid < 0) {
			perror("main: fork");
			exit(1);
		}

		if (pid == 0) {
			prctl(PR_SET_NAME, "A");
			pid = fork();
			pids[0] = pid;
			if (pid < 0) {
				perror("main: fork");
				exit(1);
			}



			if (pid == 0) {

				prctl(PR_SET_NAME, "B");
				raise(SIGSTOP);
				printf("child B exiting...\n");
				exit(1);

			}


			pid = fork();
			pids[1] = pid;
			if (pid < 0) {
				perror("main: fork");
				exit(1);
			}



			if (pid == 0) {

				prctl(PR_SET_NAME, "C");
				raise(SIGSTOP);
				printf("child C exiting\n");
				exit(1);

			}

			raise(SIGSTOP);
			kill(pids[0],SIGCONT);
			wait(&status);	
			kill(pids[1],SIGCONT);
			wait(&status);
			printf("A exiting...\n");
			exit(1);
		}

		raise(SIGSTOP);
		kill(pid,SIGCONT);
		wait(&status);
		printf("root exiting\n");
		exit(1);
	}

	show_pstree(getpid());
	//kill(pid,SIGCONT);
	wait(&status);



}
