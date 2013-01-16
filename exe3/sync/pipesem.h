
/*
 * pipesem.h
 *
 * A simple library to implement
 * semaphores over Unix pipes.
 *
 * Vangelis Koukis <vkoukis@cslab.ece.ntua.gr>
 */

#ifndef PIPESEM_H__
#define PIPESEM_H__

struct pipesem {
	/*
	 * Two file descriptors:
	 * one for the read and one for the write end of a pipe
	 */
	int rfd;
	int wfd;
};

/*
 * Function prototypes
 */
void pipesem_init(struct pipesem *sem, int val);
void pipesem_wait(struct pipesem *sem);
void pipesem_signal(struct pipesem *sem);
void pipesem_destroy(struct pipesem *sem);

#endif /* PIPESEM_H__ */
