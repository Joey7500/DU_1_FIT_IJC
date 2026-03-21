CC = gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra

all: primes primes-i no-comment

no-comment: no-comment.o error.o
	$(CC) $(CFLAGS) -o no-comment no-comment.o error.o

no-comment.o: no-comment.c
	$(CC) $(CFLAGS) -c no-comment.c -o no-comment.o

error.o: error.c
	$(CC) $(CFLAGS) -c error.c -o error.o

primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) -o primes primes.o eratosthenes.o error.o -lm

primes.o: primes.c
	$(CC) $(CFLAGS) -c primes.c -o primes.o

eratosthenes.o: eratosthenes.c
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o

primes-i: primes-i.o eratosthenes-i.o error.o
	$(CC) $(CFLAGS) -o primes-i primes-i.o eratosthenes-i.o error.o -O0 -lm

primes-i.o: primes.c
	$(CC) $(CFLAGS) -c primes.c -o primes-i.o -DUSE_INLINE -O0

eratosthenes-i.o: eratosthenes.c
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes-i.o -DUSE_INLINE -O0

run: primes primes-i
	ulimit -s 60000; ./primes ; ./primes-i
