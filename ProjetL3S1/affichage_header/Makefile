CC=gcc
CFLAGS=-Wall -Werror -g

all: main.o readelf.o
	$(CC) $(CFLAGS) -o affichage_header main.o readelf.o

clean:
	rm -f *.o

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
