#include <stdio.h>
#include <stdlib.h>

int main() {


	int val;
	scanf("%d",&val);
	int i;
	char * s = malloc(val*sizeof(char));
	for (i=0;i<val;i++)
		*(s+i)="*";
	for (i=0;i<val;i++)
		printf("%c\n",(s+i));


}
