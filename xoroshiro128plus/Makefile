CC = gcc -Wall -O3 -std=c99 -Wmissing-prototypes $(DDEBUG)

all: gen64

gen64:  gen64.c xoroshiro128plus.h splitmix64.h
	${CC} -o $@  gen64.c

.c.o:
	${CC} -c $<

clean:
	rm -rf *.o *~ *.dSYM gen64
