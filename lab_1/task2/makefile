all: base

base: base.o 
	gcc -g -m32 -Wall -o base base.o

base.o: base.c
	gcc -g -m32 -Wall -c base.c

clean: 
	rm -f *o base