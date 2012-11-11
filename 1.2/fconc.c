#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc ,char **argv) {
	
	int fd;
	if (argc <3 )	{
		printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n");
	} else {
		
		fd = open(argv[1], O_RDONLY);
		if (fd<0) {
			perror("open");
			exit(1);	
		}
		
	}

	return 0;
}
