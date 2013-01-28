#include <stdio.h>

#include "queue.h"


int main() {

	queue * q;
	q = init_queue();
	enqueue(1,q);
	printf("%d\n",dequeue(q));
	
return 0;
}
