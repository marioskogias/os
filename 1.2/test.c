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

int doRead(int fd,char * buf,int len) {
	if (read(fd, buf, len) < 0) {
		perror("Problem with reading");
		exit(1);
	}


}

void write_file(int fd, const char *infile) {
	int r;	
	char buf[BUF_SIZE];	
	int inf = open(infile, O_RDONLY); //file descriptor 1
	if (inf<0) {
		perror(infile);
		exit(1);	
	}
	while ((r = doRead(inf,buf,BUF_SIZE))>0) {
		doWrite(fd,buf,r);
	}

	close(inf);
		

}

int main(int argc ,char **argv) {
	
	int fd1,fd2,out;
	int r1;
	char * filename;
	
	if (argc <3 )	{
		printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n");
	} else {
		
		out = open(argv[2], O_WRONLY, S_IRUSR | S_IWUSR);	 // me append sto telos
		if (out<0) {
		perror("Can't create output file");
		exit(1);	
		}		
		
		write_file(out,argv[1]);
				
		close(out);
	
	}
	return 0;
}
