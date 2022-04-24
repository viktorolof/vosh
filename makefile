CC=gcc
CFLAGS = -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

voshello: vosh.c vosh.h mastermind.o
	$(CC) $(CFLAGS) -o voshell vosh.c vosh.h mastermind.o

mastermind.o: mastermind.c mastermind.h
	$(CC) $(CFLAGS) -c mastermind.c mastermind.h