#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "queue.h"
#include "process.h"

struct node {
	struct process * p;
	struct node * pre;
};

struct queue {
	struct node * head;
	struct node * tail;
	int size;
};

struct queue * init_queue() {
	struct queue * q  = malloc(sizeof(struct queue));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;	
}

void delete(pid_t p,struct queue *q) {
	struct node * temp = q->head;
	struct node * pt = NULL;
	while (temp->p->pid != p) {
		pt = temp;
		temp = temp->pre;

	}
	
		
	pt->pre = temp->pre;
	printf("deleting %d\n",temp->p->pid);
	free(temp);
	q->size--;

}
struct process * get_top(struct queue * q) {
	if (q->head == NULL) 
		return NULL;
	else  
		return q->head->p;

}
struct process * dequeue(struct queue * q) {
	struct node * temp ;
	temp = q->head;
	q->size--;

	if (q->size == 0) {
		q->head = NULL;
		q->tail = NULL;
	} else {
		q->head = temp->pre;	
	}
	
		
	return temp->p;
}

void enqueue(struct process * p, struct queue * q) {
	
	struct node * new = malloc(sizeof(struct node));
	new->p = p;
	new->pre = NULL;
	if (q->size == 0) {
		q->head = new;
		q->tail = new;
	} else {
		q->tail->pre = new;
		q->tail= new;
	}
	q->size++;
}	

char * name_by_pid(pid_t p, queue * q) {
	
	struct node * t = q->head;
	while(t->p->pid != p)
		t = t->pre;

	return t->p->name;


}
