CC = gcc
SOURCES =crawler.c
CFILES=crawler.c
CFLAGS = -Wall -pedantic -std=c99

UTILDIR=../util/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)dictionary.c $(UTILDIR)files.c $(UTILDIR)hash.c $(UTILDIR)html.c $(UTILDIR)linkedlist.c
UTILH=$(UTILDIR)dictionary.h $(UTILDIR)hash.h $(UTILDIR)html.h $(UTILDIR)header.h $(UTILDIR)crawler.h

crawler:$(SOURCES) $(UTILH) $(UTILLIB)
	$(CC) $(CFLAGS) -g -o $@ $(CFILES) -L$(UTILDIR) $(UTILFLAG)

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make;
clean:
	rm -f *~
	rm -f *#
	rm -f ./crawler
	rm -f *.o
	rm -rf ../webpages
	rm -f *.gch
	mkdir ../webpages
	rm -f ../data/webgraph.dat
