CC=gcc
CFLAGS=-Wall -Werror -g
SOURCES=$(wildcard *.c)
DEPS=$(patsubst %.c, %.dep, $(SOURCES))
OBJ=$(patsubst %.c, %.o, $(SOURCES))
NOM=our_readelf


.PHONY:clean default all

all: $(NOM)

clean:
	rm *.o *.dep $(NOM)

$(NOM): $(OBJ)
	$(CC) -o $@ $^

-include $(DEPS)

%.dep: %.c
	gcc -MM $< >$@

