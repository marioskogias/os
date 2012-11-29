#include <unistd.h>
#include <stdio.h>
#include <string.h>

void zing(void) {
	//printf("Hello from my new zing %s\n",getlogin());
	char buf1[] = "Hello from my new zing ";
	write(1, buf1, strlen(buf1));
	char * buf2 = getlogin();
	write(1,buf2,strlen(buf2));
	write(1, "\n", 1);
	
}
