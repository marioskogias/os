zing : main.o zing.o
	gcc main.o zing.o -o zing

main.o : main.c
	gcc main.c -Wall -c main.o
