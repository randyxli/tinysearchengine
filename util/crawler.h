#include "../util/dictionary.h"
#ifndef _CRAWLER_H_
#define _CRAWLER_H_

// *****************Impementation Spec********************************
// File: crawler.c
// This file contains useful information for implementing the crawler:
// - DEFINES
// - MACROS
// - DATA STRUCTURES
// - PROTOTYPES

// DEFINES

// The max length of each URL path.
#define MAX_URL_LENGTH 1000

#define INTERVAL_PER_FETCH 1

// The URL we crawled should only starts with this prefix. 
// You could remove this limtation but not before testing.
// The danger is a site may block you
#define URL_PREFIX "http://www.cs.dartmouth.edu"

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

// MAX_URL_PER_PAGE is no longer relevant thanks to extractURLsLL. 
#define MAX_URL_PER_PAGE 100

//file that wget.sh downloads webpages to for temporary storage
#define CURRENTPAGE "./currentpage.html"

//these two are currently not being used, but are saved in case I want to bring back my depth-first search
#define HISTORY 1
#define OFFSET 0

// DATA STRUCTURES. All these structures should be malloc 'd

//Crawlparams holds three pieces of widely used data: DICTIONARY, the path where crawled webpages are saved, and the maximum search depth.


//this structure is for painless altering of function parameters
typedef struct _crawlparams
{
     DICTIONARY *dict;
     char *path;
     int maxdepth;
} Crawlparams;

//PROTOTYPES used by crawler.c You have to code them.

// getPage: Assumption: if you are dowloading the file it must be unique. 
// Two cases. First its the SEED URL so its unique. Second, wget only getpage 
//once a URL is computed to be unique. Get the HTML file saved in TEMP 
// and read it into a string that is returned by getPage. Store TEMP
// to a file 1..N and save the URL and depth on the first and second 
// line respectively.

char *getPage(char* url, int depth,  char* path);

// extractURL: Given a string of the HTML page, parse it (you have the code 
// for this GetNextURL) and store all the URLs in the url_list, implemented
// as a queue.
DNODE *extractURLsLL(char *html_buffer, char *current, DNODE *anchor, int depth);
#endif
