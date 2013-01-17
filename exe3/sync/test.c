#include <stdio.h>


int main() {


	int a[2][3];
	int i;
	for (i=0;i<4;i++)
		a[0] = i;

	for (i=0;i<2;i++)
		printf("%d ", a[0][i]);

	printf("\n");
	
	for (i=0;i<2;i++)
		printf("%d ", a[1][i]);
}
