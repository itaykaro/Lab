all: AntiVirus

AntiVirus.o: AntiVirus.c
	gcc -g -m32 -Wall -c AntiVirus.c

AntiVirus: AntiVirus.o
	gcc -g -m32 -Wall -o AntiVirus AntiVirus.o

clean:
	rm *.o AntiVirus