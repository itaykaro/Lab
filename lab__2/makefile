all: myshell

myshell: myshell.o LineParser.o
	gcc -g -m32 -Wall -o myshell myshell.o LineParser.o

myshell.o: myshell.c
	gcc -g -m32 -Wall -c myshell.c

LineParser.o: LineParser.c LineParser.h
	gcc -g -m32 -Wall -o LineParser.o LineParser.c LineParser.h

clean: 
	rm *.o myshell