#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct queue queue;

queue * init_queue() ;

int dequeue(queue * q);

void enqueue(int pro,queue * q);

#endif /* REQUEST_H_ */


