#ifndef QUEUE_H_
#define QUEUE_H_

/*η βασική δομή της ουράς*/
typedef struct queue queue;

/*ο "constructor"*/
queue * init_queue() ;

/*αφαίρεσε την πρώτη διεργασία*/
struct process * dequeue(queue * q);

/*επέστρεψε την πρώτη διεργασία*/
struct process * get_top(queue *q);

/*τοποθέτησε την δοσμένη διεργασία στο τέλος της ουράς */
void enqueue(struct process * p,queue * q);

/*διέγραψε τη διεργασία με το pid p*/
void delete(pid_t p, queue * q);

/*επέστρεψε το όνομα της διεργασίας με το pid p*/
char * name_by_pid(pid_t p,queue * q);

#endif /* REQUEST_H_ */


