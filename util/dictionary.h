#include <stddef.h>

#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

// DEFINES

// The max length of each URL path.
#define MAX_URL_LENGTH 1000

// Set some magic numbers. These are large values.

// Unlikely to have more than an a URL longer that 1000 chars.
// The KEY is the same as a URL. The term KEY is just a
// general Dictionary/hash function term 
#define KEY_LENGTH 1000

// Make the hash large in comparison to the number of URLs found
// for example depth of 2 on www.cs.dartmouth.edu approx 200.
// This will minimize collisions and mostly likely slots will be
// empty of have one or two DNODES. Access is O(1). Fast.

//make sure this is less than the maximum integer value, or else crashes will happen
#define MAX_HASH_SLOT 10000

typedef struct _URL{
  char url[MAX_URL_LENGTH];      // e.g., www.cs.dartmouth.edu
  int depth;                     //  depth associated with this URL.
} __URL;

typedef struct _URL URLNODE;

typedef struct _DNODE {
  struct _DNODE  *next;
  void    *data;        //  actual data points to URLNOD
     int     depth;
  char key[KEY_LENGTH]; //  actual (URL) key 
} __DNODE;

typedef struct _DNODE DNODE;

// The DICTIONARY holds the hash table and the start and end pointers into a double 
// link list. This is a unordered list with the exception that DNODES with the same key (URL) 
// are clusters along the list. So you hash into the list. Check for uniqueness of the URL.
// If not found add to the end of the cluster associated wit the same URL. You will have
// to write an addElement function.

typedef struct _DICTIONARY {
  DNODE *hash[MAX_HASH_SLOT]; // the hash table of slots, each slot points to a DNODE
} __DICTIONARY;


typedef struct _DICTIONARY DICTIONARY;

DNODE *uniqueKey(DICTIONARY *dict, char *newkey);

int addWordToDict(DICTIONARY *dict, char *word, char *newurl);

int addToDict(DICTIONARY *dict, int newdepth, char *newurl);

void printq(DNODE *q);

void dequeue(DNODE **q);

void freeLL(DNODE *node);

void freeNestedLL(DNODE *node);

DNODE *searchLL(DNODE *head, char *newkey);

DNODE *copyLL(DNODE *head);

DNODE *sortLL(DNODE *head);

DNODE *AND(DNODE *operand1, DNODE *operand2);

DNODE *OR(DNODE *operand1, DNODE *operand2);

void freeDict(DICTIONARY **dict);

void freeWordDict(DICTIONARY **dict);

char *DNODEToString(DNODE *node);

DICTIONARY *loadDict(char *datfile);

void saveDict(char *filename, DICTIONARY *dict);

void appendToFile(char *file, char *str);

char *getFileContents(char *filepath);

void freeDictCR(DICTIONARY **dict);
#endif
