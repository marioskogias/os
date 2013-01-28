#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

#include <sys/wait.h>
#include <sys/types.h>

#include "proc-common.h"
#include "request.h"
#include "queue.h"
/* Compile-time parameters. */
#define SCHED_TQ_SEC 2                /* time quantum */
#define TASK_NAME_SZ 60               /* maximum size for a task's name */

/*the process queue*/
queue * q;

/*
 * SIGALRM handler
 */
static void
sigalrm_handler(int signum){
	pid_t p;
	p = get_top(q);
	kill(p,SIGSTOP);
	printf("in alarm handler\n");	
	
	 if (alarm(SCHED_TQ_SEC) < 0) {
                perror("alarm");
                exit(1);
        }

	//assert(0 && "Please fill me!");
}

/* 
 * SIGCHLD handler
 */
static void
sigchld_handler(int signum){

	if (signum != SIGCHLD) {
                fprintf(stderr, "Internal error: Called for signum %d, not SIGCHLD\n",
                        signum);
                exit(1);
        }
	
	pid_t p;
	int status;
	for (;;) {
                p = waitpid(-1, &status, WUNTRACED | WNOHANG);
                if (p < 0) {
                        perror("waitpid");
                        exit(1);
                }
                if (p == 0)
                        break;

                explain_wait_status(p, status);

                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                        /* A child has died */
                        printf("Parent: Received SIGCHLD, child is dead.\n");
                        dequeue(q);
			p = get_top(q);
			kill(p,SIGCONT);
		}
		if (WIFSTOPPED(status)) {
			/* A child has stopped due to SIGSTOP/SIGTSTP, etc... */
			printf("Parent: Child has been stopped. Moving right along...\n");
			p = dequeue(q);
			enqueue(p,q);
			p = get_top(q);
			kill(p,SIGCONT);
		}
        }
	//assert(0 && "Please fill me!");
}

/* Install two signal handlers.
 * One for SIGCHLD, one for SIGALRM.
 * Make sure both signals are masked when one of them is running.
 */
static void
install_signal_handlers(void)
{
	sigset_t sigset;
	struct sigaction sa;

	sa.sa_handler = sigchld_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGCHLD);
	sigaddset(&sigset, SIGALRM);
	sa.sa_mask = sigset;
	if (sigaction(SIGCHLD, &sa, NULL) < 0) {
		perror("sigaction: sigchld");
		exit(1);
	}

	sa.sa_handler = sigalrm_handler;
	if (sigaction(SIGALRM, &sa, NULL) < 0) {
		perror("sigaction: sigalrm");
		exit(1);
	}

	/*
	 * Ignore SIGPIPE, so that write()s to pipes
	 * with no reader do not result in us being killed,
	 * and write() returns EPIPE instead.
	 */
	if (signal(SIGPIPE, SIG_IGN) < 0) {
		perror("signal: sigpipe");
		exit(1);
	}
}




int main(int argc, char *argv[])
{
	int nproc;
	/*
	 * For each of argv[1] to argv[argc - 1],
	 * create a new child process, add it to the process list.
	 */
	int i;
	pid_t p;
	q = init_queue();
	for (i=1;i<argc;i++) {
		p = fork();
		if (p < 0) {
			perror("fork");
			exit(1);
		}

		if (p == 0) {
			char *newargv[] = { argv[i], NULL, NULL, NULL };
        		char *newenviron[] = { NULL };
			raise(SIGSTOP);
			execve(argv[i],newargv,newenviron);
			exit(1);
		}
		
		enqueue(p,q);
		
	}	

	nproc = argc-1; /* number of proccesses goes here */

	/* Wait for all children to raise SIGSTOP before exec()ing. */
	wait_for_ready_children(nproc);

	/* Install SIGALRM and SIGCHLD handlers. */
	install_signal_handlers();

	/* Install SIGCHLD handler */
        if (signal(SIGCHLD, sigchld_handler) < 0) {
                perror("signal");
                exit(1);
        }

        /* Install SIGALRM handler */
        if (signal(SIGALRM, sigalrm_handler) < 0) {
                perror("signal");
                exit(1);
        }

        /* Arrange for an alarm after 1 sec */
        if (alarm(SCHED_TQ_SEC) < 0) {
                perror("alarm");
                exit(1);
        }


	/*start the process*/
	p = get_top(q);
	kill(p,SIGCONT);

	if (nproc == 0) {
		fprintf(stderr, "Scheduler: No tasks. Exiting...\n");
		exit(1);
	}


	/* loop forever  until we exit from inside a signal handler. */
	while (pause())
		;

	/* Unreachable */
	fprintf(stderr, "Internal error: Reached unreachable point\n");
	return 1;
}
