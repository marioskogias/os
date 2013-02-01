#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct queue queue;

queue * init_queue() ;

struct process * dequeue(queue * q);

struct process * get_top(queue *q);

void enqueue(struct process *,queue * q);

void delete(pid_t p, queue * q);

char * name_by_pid(pid_t p,queue * q);

#endif /* REQUEST_H_ */


