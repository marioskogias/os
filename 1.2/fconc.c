#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>

#define BUF_SIZE 1

int main(int argc ,char **argv) {
	
	int fd1,fd2,out;
	int r1;
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
				
		out = open(filename,O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);	
		
		char buf[BUF_SIZE];

		while ((r1 = read(fd1,buf,BUF_SIZE))>0)
			write(out, buf, r1);

		while ((r1 = read(fd2,buf,BUF_SIZE))>0)
			write(out, buf, r1);
		
	
		close(fd1);
		close(fd2);
		close(out);
	
	}
	return 0;
}
