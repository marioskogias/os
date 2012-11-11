#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>
#include <sys/stat.h>


#define BUF_SIZE 256

void doWrite(int fd, const char *buff, int len) {
	
	if (write(fd, buff, len) != len) {
		perror("couldn't write whole buffer");
		exit(1);
	}

}

void write_file(int fd, const char *infile) {
	int inf = open(infile, O_RDONLY); //file descriptor 1
	int r;	
	char buf[BUF_SIZE];
	if (inf<0) {
		perror(infile);
		exit(1);	
	}
	while ((r = read(fd,buf,BUF_SIZE))>0)
		doWrite(fd,buf,r);

	close(inf);
		

}
int main(int argc ,char **argv) {
	
	int fd1,fd2,out;
	int r1;
	char * filename;
	
	if (argc <3 )	{
		printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n");
	} else {
		
		
		out = open(filename,O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);	
		if (out<0) {
		perror("Can't create output file");
		exit(1);	
		}		

		write_file(out,argv[1]);
		write_file(out,argv[2]);
		
		close(out);
	
	}
	return 0;
}
