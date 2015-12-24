CC = gcc -Wall -O3 -std=c99

all: mt19937-64test

mt19937-64test: mt19937-64test.c mt19937-64.o
	${CC} -o $@ mt19937-64test.c mt19937-64.o

.c.o:
	${CC} -c $<

clean:
	rm -rf *.o *~ *.dSYM mt19937-64test
