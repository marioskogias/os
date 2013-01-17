/*
 * mandel.c
 *
 * A program to draw the Mandelbrot Set on a 256-color xterm.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "pipesem.h"
#include "proc-common.h"
#include "mandel-lib.h"

#define MANDEL_MAX_ITERATION 100000

/***************************
 * Compile-time parameters *
 ***************************/

/*
 * Output at the terminal is is x_chars wide by y_chars long
*/
int y_chars = 51;
int x_chars = 90;

/*
 * The part of the complex plane to be drawn:
 * upper left corner is (xmin, ymax), lower right corner is (xmax, ymin)
*/
double xmin = -1.8, xmax = 1.0;
double ymin = -1.0, ymax = 1.0;
	
/*
 * Every character in the final output is
 * xstep x ystep units wide on the complex plane.
 */
double xstep;
double ystep;

/*
 * This function computes a line of output
 * as an array of x_char color values.
 */
void compute_mandel_line(int line, int color_val[])
{
	/*
	 * x and y traverse the complex plane.
	 */
	double x, y;

	int n;
	int val;

	/* Find out the y value corresponding to this line */
	y = ymax - ystep * line;

	/* and iterate for all points on this line */
	for (x = xmin, n = 0; x <= xmax; x+= xstep, n++) {

		/* Compute the point's color value */
		val = mandel_iterations_at_point(x, y, MANDEL_MAX_ITERATION);
		if (val > 255)
			val = 255;

		/* And store it in the color_val[] array */
		val = xterm_color(val);
		color_val[n] = val;
	}
}

/*
 * This function outputs an array of x_char color values
 * to a 256-color xterm.
 */
void output_mandel_line(int fd, int color_val[])
{
	int i;
	
	char point ='@';
	char newline='\n';

	for (i = 0; i < x_chars; i++) {
		/* Set the current color, then output the point */
		set_xterm_color(fd, color_val[i]);
		if (write(fd, &point, 1) != 1) {
			perror("compute_and_output_mandel_line: write point");
			exit(1);
		}
	}

	/* Now that the line is done, output a newline character */
	if (write(fd, &newline, 1) != 1) {
		perror("compute_and_output_mandel_line: write newline");
		exit(1);
	}
}

void compute_and_output_mandel_line(int fd, int line)
{
	/*
	 * A temporary array, used to hold color values for the line being drawn
	 */
	int color_val[x_chars];

	compute_mandel_line(line, color_val);
	output_mandel_line(fd, color_val);
}

int main(void)
{
	int line=0;

	xstep = (xmax - xmin) / x_chars;
	ystep = (ymax - ymin) / y_chars;
	
	struct pipesem  a;
	struct pipesem  b;
	struct pipesem  c;
	
	pipesem_init(&a,1);
	pipesem_init(&b,0);
	pipesem_init(&c,0);
	
	pid_t pid;
	int status;
	int i;
//	int color_val[x_chars];
	
	int color_val[y_chars/3][x_chars];

	pid = fork();
	if (pid < 0) {
                perror("main: fork");
                exit(1);
	}
	if (pid == 0) {
		/*prwth diergasia ypologismou*/
		i=0;
		while (line<y_chars){	
			compute_mandel_line(line,color_val[i]);	
			line = line + 3;
			i++;
		}
		i=0;
		while(i<y_chars/3) {
			pipesem_wait(&a);
			output_mandel_line(1,color_val[i]);
			pipesem_signal(&b);
			i++;
		}
		exit(1);
	}

	line++;
	pid = fork();
	if (pid < 0) {
                perror("main: fork");
                exit(1);
        }
	if (pid == 0) { 
		/*deuterh diergasia ypologismou*/	
		i=0;
		while (line<y_chars){		
			compute_mandel_line(line,color_val[i]);	
			line = line + 3;
			i++;	
		}
		i=0;
		while(i<y_chars/3) {
			pipesem_wait(&b);
			output_mandel_line(1,color_val[i]);
			pipesem_signal(&c);
			i++;
		}
		exit(1);
	}


	line++;
	pid = fork();
	if (pid < 0) {
                perror("main: fork");
                exit(1);
        }
	if (pid == 0) { 
		/*trith diergasia ypologismou*/
		i=0;
		while (line<y_chars){		
			compute_mandel_line(line,color_val[i]);	
			line = line + 3;
			i++;
		}
		i=0;
		while(i<y_chars/3) {
			pipesem_wait(&c);
			output_mandel_line(1,color_val[i]);
			pipesem_signal(&a);
			i++;
		}
		exit(1);
	}

	pid = wait(&status);
       // explain_wait_status(pid, status);

	pid = wait(&status);
       // explain_wait_status(pid, status);

	pid = wait(&status);
       // explain_wait_status(pid, status);
	

	reset_xterm_color(1);
	return 0;
}
