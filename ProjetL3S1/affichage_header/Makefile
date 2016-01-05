CC=gcc
CFLAGS=-Wall -Werror -g

all: main.o readelf.o printelf.o
	$(CC) $(CFLAGS) -o affichage_header main.o readelf.o printelf.o

clean:
	rm -f *.o

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
