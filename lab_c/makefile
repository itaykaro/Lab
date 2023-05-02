all: myshell mypipe

myshell: myshell.o
	gcc -g -m32 -Wall -o myshell myshell.o 

myshell.o: myshell.c LineParser.c LineParser.h
	gcc -g -m32 -Wall -c myshell.c LineParser.c

mypipeline: mypipeline.o
	gcc -g -m32 -Wall -o mypipeline mypipeline.o 

mypipeline.o: mypipeline.c 
	gcc -g -m32 -Wall -c mypipeline.c 

clean: 
	rm *.o myshell