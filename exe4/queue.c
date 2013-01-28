#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct node {
	int val;
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

int dequeue(struct queue * q) {
	struct node * temp ;
	int val;
	temp = q->head;
	val = q->head->val;
	q->size--;

	if (q->size == 0) {
		q->head = NULL;
		q->tail = NULL;
	} else {
		q->head = temp->pre;	
	}
	
	free(temp);
		
	return val;
}

void enqueue(int pro, struct queue * q) {
	struct node * new = malloc(sizeof(struct node));
	new->val = pro;
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


