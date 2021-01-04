CC=gcc

all: matrixcalc

matrixcalc: matrixcalc.c 
	$(CC) -o matrixcalc matrixcalc.c -l pthread
clean:
	rm matrixcalc
