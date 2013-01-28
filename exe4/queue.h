#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct queue queue;

queue * init_queue() ;

pid_t dequeue(queue * q);

pid_t get_top(queue *q);

void enqueue(pid_t pro,queue * q);

#endif /* REQUEST_H_ */


