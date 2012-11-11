#include <unistd.h>
#include <stdio.h>

void zing(void) {
	printf("Hello from my new zing %s\n",getlogin());
}
