CC=gcc
CFLAGS=-Wall -Werror -g

all: main.o
	$(CC) $(CFLAGS) -o affichage_header main.o

clean:
	rm -f *.o

main.o: main.c
	$(CC) $(CFLAGS) -c $<
