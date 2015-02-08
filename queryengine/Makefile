CC = gcc
SOURCES = query.c
CFILES=query.c
CFLAGS = -Wall -pedantic -std=c99 -g

UTILDIR=../util/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)dictionary.c $(UTILDIR)files.c $(UTILDIR)hash.c $(UTILDIR)html.c $(UTILDIR)linkedlist.c
UTILH=$(UTILDIR)dictionary.h $(UTILDIR)hash.h $(UTILDIR)html.h $(UTILDIR)header.h 

query:$(SOURCES) $(UTILH) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(CFILES) -L$(UTILDIR) $(UTILFLAG)

test: query_test.c
	$(CC) $(CFLAGS) -o @test query_test.c -L$(UTILDIR) $(UTILFLAG)

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make;
clean:
	rm -f *~
	rm -f *#
	rm -f *.o
	rm -f *.gch
	rm -f *.tmp
	rm -f query
	rm -f @test
