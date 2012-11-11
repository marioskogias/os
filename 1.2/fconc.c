#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>

int main(int argc ,char **argv) {
	
	int fd1,fd2,out;
	char * filename;
	if (argc <3 )	{
		printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n");
	} else {
		
		fd1 = open(argv[1], O_RDONLY); //file descriptor 1
		if (fd1<0) {
			perror(argv[1]);
			exit(1);	
		}
		fd2 = open(argv[2], O_RDONLY); //file descriptor 2
		if (fd2<0) {
			perror(argv[2]);
			exit(1);	
		}
		
		if (argc == 3) 
			filename = "fconc.out";
		else 
			filename = argv[3];
			
		out = open(filename,O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
		char buf[] = "kalhmera";
		write(out, buf, strlen(buf));
	
		close(fd1);
		close(fd2);
		close(out);
	
	}
	return 0;
}
