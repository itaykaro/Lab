all: encoder

encoder: encoder.o
	gcc -m32 -Wall -o encoder encoder.o

encoder.o: encoder.c
	gcc -m32 -Wall -c encoder.c

clean:
	rm -f *.o encoder