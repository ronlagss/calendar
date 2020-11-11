CC=gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors \
	-fstack-protector-all -Wextra

PROGS = tests

#executables

tests : tests.o  calendar.o
	$(CC) tests.o calendar.o -o tests


#objects

tests.o : tests.c event.h calendar.h
	$(CC) $(CFLAGS) -c tests.c

calendar.o : calendar.c calendar.h event.h
	$(CC) $(CFLAGS) -c calendar.c


#all object files and executables
.PHONY: all clean

all: $(PROGS)

clean:
	rm -f *.o $(PROGS)
