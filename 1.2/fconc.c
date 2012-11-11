#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>

#define BUF_SIZE 256

void doWrite(int fd, const char *buff, int len) {
	write(fd, buff, len);
}

void write_file(int fd, const char *infile) {
	int inf = open(infile, O_RDONLY); //file descriptor 1
	char buf[BUF_SIZE];
	if (inf<0) {
		perror(infile);
		exit(1);	
	}
	while ((r1 = read(fd1,buf,BUF_SIZE))>0)
		doWrite(fd,buff,r1)

}
int main(int argc ,char **argv) {
	
	int fd1,fd2,out;
	int r1;
	char * filename;
	
	if (argc <3 )	{
		printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n");
	} else {
		
		
		out = open(filename,O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);	
		write_file
				
		
		
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
