all: read_bin

read_bin.o: read_bin.c
	gcc -m32 -Wall -c read_bin.c

read_bin: read_bin.o
	gcc -m32 -Wall -o read_bin read_bin.o

clean:
	rm *.o read_bin