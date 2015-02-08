CC = gcc
SOURCES = files.c header.h linkedlist.c dictionary.h html.c html.h hash.c hash.h dictionary.c
OBJS=dictionary.o hash.o html.o linkedlist.o files.o
CFLAGS = -Wall -pedantic -std=c99 -g

objs: $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)

clean:
	rm -f *~
	rm -f *#
	rm -f *.o
	rm -f *.gch
	rm -f *.dat
	rm -f *.tmp
lib:
	rm -f libtseutil.a
	ar -cvq libtseutil.a $(OBJS)