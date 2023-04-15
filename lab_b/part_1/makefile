all: virusDetector

AntiVirus.o: AntiVirus.c
	gcc -g -m32 -Wall -c AntiVirus.c

virusDetector : AntiVirus.o
	gcc -g -m32 -Wall -o virusDetector AntiVirus.o

clean:
	rm *.o virusDetector 