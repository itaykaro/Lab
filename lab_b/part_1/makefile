all: part_1

part_1.o: part_1.c
	gcc -g -m32 -Wall -c part_1.c

part_1: part_1.o
	gcc -g -m32 -Wall -o part_1 part_1.o

clean:
	rm *.o part_1