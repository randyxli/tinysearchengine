CC = gcc
SOURCES =indexer.c
CFILES=indexer.c
CFLAGS = -Wall -pedantic -std=c99

UTILDIR=../util/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)dictionary.c $(UTILDIR)files.c $(UTILDIR)hash.c $(UTILDIR)html.c $(UTILDIR)linkedlist.c
UTILH=$(UTILDIR)dictionary.h $(UTILDIR)hash.h $(UTILDIR)html.h $(UTILDIR)header.h 

indexer:$(SOURCES) $(UTILH) $(UTILLIB)
	$(CC) $(CFLAGS) -g -o $@ $(CFILES) -L$(UTILDIR) $(UTILFLAG)
clean:
	rm -f *~
	rm -f *#
	rm -f *.o
	rm -f *.gch
	rm -f *.dat
	rm -f *.tmp
	rm -f ../data/index.dat