all: myshell mypipe

myshell: myshell.o
	gcc -g -m32 -Wall -o myshell myshell.o 

myshell.o: myshell.c LineParser.c LineParser.h
	gcc -g -m32 -Wall -c myshell.c LineParser.c

mypipe: mypipe.o
	gcc -g -m32 -Wall -o mypipe mypipe.o 

mypipe.o: mypipe.c 
	gcc -g -m32 -Wall -c mypipe.c 

clean: 
	rm *.o myshell