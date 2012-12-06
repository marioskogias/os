.PHONY: clean

default: zing

zing : main.o zing2.o
	gcc main.o zing2.o -o zing2
	gcc main.o zing.o -o zing

main.o : main.c
	gcc main.c -Wall -c

zing2.o : zing2.c
	gcc zing2.c -Wall -c
clean: 
	rm zing2.o zing zing2 main.o
