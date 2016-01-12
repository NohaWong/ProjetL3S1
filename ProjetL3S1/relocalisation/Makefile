CC=gcc
CFLAGS=-Wall -Werror -g -I../commun
SOURCES=$(wildcard *.c wildcard ../commun/*.c )
DEPS=$(patsubst %.c, %.dep, $(SOURCES))
OBJ=$(patsubst %.c, %.o, $(SOURCES))
NOM=our_relocalisation


.PHONY:clean default all

all: $(NOM)

clean:
	rm *.o *.dep $(NOM)

$(NOM): $(OBJ)
	$(CC) -o $@ $^

-include $(DEPS)

%.dep: %.c
	gcc -MM $< >$@

