all: myshell

myshell: myshell.o
	gcc -g -m32 -Wall -o myshell myshell.o 

myshell.o: myshell.c LineParser.c LineParser.h
	gcc -g -m32 -Wall -c myshell.c LineParser.c

clean: 
	rm *.o myshell