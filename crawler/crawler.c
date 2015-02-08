/*

  FILE: crawler.c

  Programmer: Randy Li

  Inputs: ./crawler [SEED URL] [TARGET DIRECTORY WHERE TO PUT THE DATA] [MAX CRAWLING DEPTH]

  Outputs: For each webpage crawled the crawler program will create a file in the 
  [TARGET DIRECTORY]. The name of the file will start a 1 for the  [SEED URL] 
  and be incremented for each subsequent HTML webpage crawled. 

  Each file (e.g., 10) will include the URL associated with the saved webpage and the
  depth of search in the file. The URL will be on the first line of the file 
  and the depth on the second line. The HTML will for the webpage 
  will start on the third line.

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <unistd.h>
#include "../util/crawler.h"
#include "../util/html.h"
#include "../util/hash.h"
#include "../util/header.h"
#include "../util/dictionary.h"



// Define the dict structure that holds the hash table 
// and the double linked list of DNODES. Each DNODE holds
// a pointer to a URLNODE. This list is used to store
// unique URLs. The search time for this list is O(n).
// To speed that up to O(1) we use the hash table. The
// hash table holds pointers into the list where 
// DNODES with the same key are maintained, assuming
// the hash(key) is not NULL (which implies the URL has
// not been seen before). The hash table provide quick
// access to the point in the list that is relevant
// to the current URL search. 


/*
Description: Attempts to download a specified webpage to a specified location. It names the saved files sequentially and adds the depth where the page was found and the URL of the page.

Input: char *url contains the URL that is to be downloaded. int depth is the depth of the crawl that produced url. char *path is the filepath where webpages are downloaded to.

Output: A string containing the full contents of the downloaded webpage. If the webpage could not be downloaded, a NULL pointer is returned.

Outputf: char *page

*****PSEUDO CODE*****
(0) if(first run of getPage)
        pagecount = 0
    endif
(1) Attempt to download page to file CURRENTPAGE (defined in crawler.h).
(2) if (download fails)
       return NULL
    endif
(3) page = contents of CURRENTPAGE
(4) Copy CURRENTPAGE, url, and depth >> path/pagecount.html
(5) Increment pagecount
(6) return page

TODO: determine what happens when wget returns negative exit codes
 */
char *getPage(char *url, int depth, char *path)
{
     static int pagecount = 0;//(1)
     char command[MAX_URL_LENGTH] = "";//holds commands that will be pashed to the bash shell

     //(1) Attempting to download from url
     snprintf(command, sizeof(command), "./wget.sh \"%s\"", url);//creating wget command
     if(system(command) > 0)//(2) if the download fails return NULL
     {
	  return NULL;
     }

     //(3) Reading contents of CURRENTPAGE into page.
     //finding size of CURRENTPAGE using fseek
     int currentsize = 0;
     FILE *fp = fopen(CURRENTPAGE,"r");
     fseek(fp, 0, SEEK_END);
     currentsize = ftell(fp);
     fseek(fp,0, SEEK_SET);
     
     //read CURRENTPAGE into page
     char *page = (char *) malloc(currentsize+1);
     MALLOC_CHECK(page);
     fread(page, 1, currentsize, fp);
     fclose(fp);
     page[currentsize] = '\0';
     
     //(4) Copying stuff into pagecount.html

     char filename[sizeof(path)+38];
     sprintf(filename, "%s%d.html", path, pagecount);//construct absolute path of pagecount.html
     
     snprintf(command, sizeof(command), "echo \"%s\" >> %s", url, filename);     
     system(command);//print url in html file
     
     snprintf(command, sizeof(command), "echo %d >> %s", depth, filename);
     system(command);//print depth in html file

     snprintf(command, sizeof(command), "cat %s >> %s", CURRENTPAGE, filename);     
     system(command);//copy CURRENTPAGE into html file

     //(5)
     pagecount++;
     return page;
}


/*
Description: Extracts URLs from a webpage and enters them into a queue.

Input: char *html_buffer holds the contents of the webpage that is being mined for URLs. char *current is the URL of the contents of html_buffer. DNODE *anchor points to the end of the queue that the URLs wil be entered in. int depth is the depth of the URLs being entered into the queue.

OUtput: A pointer that points to the last element in the queue.

OutputR: all the DNODEs attached to anchor

*****PSEUDO CODE*****
(1) while(there are more URLs to be extracted)
        url_buffer = getNextURL
(2)	if(URL is valid and begins with http://www.cs.dartmouth.edu)            
(3)         Create new queue element that contains url_buffer
(4)         Add new element into queue
        endif
    endwhile

    TODO: write enqueue function to make code more readable
*/
DNODE *extractURLsLL(char *html_buffer, char *current, DNODE *anchor, int depth)
{
     char url_buffer[MAX_URL_LENGTH];
     BZERO(url_buffer, MAX_URL_LENGTH);
     
     int i=0;
     int pos=0;
     DNODE *tail = anchor; //= (DNODE *) calloc(1,sizeof(DNODE *));
	  //MALLOC_CHECK(tail);
     DNODE *newnode = NULL;

     //(1)
     while((pos = GetNextURL(html_buffer, current, url_buffer, pos))>0)
     {
	  //(2)
	  if(NormalizeURL(url_buffer)==1)//checks validity of url_buffer and normalizes it
	  {
	       //if((0==strncmp(URL_PREFIX,url_buffer,strlen(URL_PREFIX))))//checks if url_buffer begins with http://www.cs.dartmouth.edu
	       //{
		    //(3) initialize new DNODE and point newnode to it
		    newnode = (DNODE *) malloc(sizeof(DNODE));
		    MALLOC_CHECK(newnode);
		    strncpy(newnode->key,url_buffer, MAX_URL_LENGTH);
		    newnode->next = NULL;
		    newnode->depth = depth;
		    newnode->data = malloc(strlen(current)+1);
		    //memcpy(newnode->data, current, sizeof(current));
		    strncpy((char *)(newnode->data), current, strlen(current)+1);

		    //(4) link up the new node to the back of the queue
		    tail->next = newnode;
		    tail = newnode; //point tail te the back of the queue		    
		    i++;
	       //}
	  }
	  BZERO(url_buffer, MAX_URL_LENGTH);
     }
     return newnode;
}



/*
Description: bfcrawl is a breadth-first search that crawls the cs.dartmouth.edu website. It downloads all pages it finds within a specified depth of the seed URL to a specified directory and adds their URLs into a DICTIONARY. Note: I did not sleep between downloads. cs.dartmouth.edu has not banned me yet even when I downloaded the same page thousands of times, so I doubt it will ban me anytime soon.

Input: Crawlparams *c holds the DICTIONARY, maximum search depth, and save directory for webpages. char *seedurl is the seedurl for the crawl.

Output: None

*****PSEUDO CODE*****
(1) Initialize queue of URLs to be visited, q.
(2) while(q is not empty)
        currurl =  URL at the front of the queue
(3)     if(currurl is not in DICTIONARY)
            return to start of the loop
        endif
(4)     page = getPage(currurl)
(5)     if(page = NULL)
            return to start of the loop
(6)     addToDictionary(currurl)
(7)     if(currurl could not be added to DICTIONARY)
            return to start of the loop
        endif
(8)     if(current depth < maximum depth)
            extractURLs(page) and enter them into q
        endif
    endwhile
    TODO: get rid of redundant uniqueness check
 */
void bfcrawl(Crawlparams *c, char* seedurl)
{
     int currdepth = 0;

     //(1) intialize the queue
     DNODE *q = (DNODE *) malloc(sizeof(DNODE));
     MALLOC_CHECK(q);
     strncpy(q->key, seedurl, KEY_LENGTH);     
     q->depth = currdepth;
     q->next = NULL;
     q->data = malloc(sizeof(seedurl));
     memcpy(q->data, seedurl, sizeof(seedurl));

     //endofq points to the last element in the queue
     DNODE *endofq = q;
     char *currurl = (char *) malloc(MAX_URL_LENGTH);
     char parenturl[MAX_URL_LENGTH];
     DNODE *temp;
		
     //(2)
     while(q!=NULL)
     {	  	  
	  strncpy(currurl,q->key,MAX_URL_LENGTH);//getting data from front of the queue
	  currdepth = q->depth;
	  strncpy(parenturl, (char *)(q->data), MAX_URL_LENGTH);

	  //(3) if currurl is already in the dictionary move to the next URL in the queue
	  if(uniqueKey(c->dict, currurl))
	  {
	       addWordToDict(c->dict, currurl, parenturl);
	       free(q->data);
	       dequeue(&q);
	       continue;
	  }
	  
	  //(4)
	  char *page = getPage(currurl, currdepth, c->path);
	  //(5) if the download for currurl fails we move onto the next URL in the queue
	  if(page==NULL)
	  {
	       free(q->data);
	       dequeue(&q);
	       continue;
	  }	 
     
	  //(6),(7) adding the URL to the dictionary
	  if(addWordToDict(c->dict, currurl, parenturl)!=0)
	  {
	       free(q->data);
	       dequeue(&q);
	       continue;
	  }
	  
	  //(8) don't extract URLs if you're not going to crawl them
	  if(currdepth<c->maxdepth)
	       if((temp=extractURLsLL(page, currurl, endofq, currdepth+1))!=NULL)//not checking for this will cause the queue to be truncated at the current endofq
	       {
		    endofq = temp;
	       }
	  free(q->data);
	  dequeue(&q);
	  free(page);	   
     }
     free(currurl);
}


/*
See top of this file for documentation.

*****PSEUDO CODE*****
(1) Validate input *yawn*
(2) Call bfcrawl
 */
int main(int argc, char *argv[])
{
     DICTIONARY *dict = (DICTIONARY *) calloc(1,sizeof(DICTIONARY));
     char seedurl[MAX_URL_LENGTH];
     BZERO(seedurl,MAX_URL_LENGTH);
     char *path;
     int maxdepth = 0;
     char command[MAX_URL_LENGTH];
     

     if(argc != 4)
     {
	  printf("Error: invalid arguments. Correct syntax is \"./crawler [SEED URL] [TARGET DIRECTORY WHERE TO PUT THE DATA] [MAX CRAWLING DEPTH]\"\n");
	  return 1;
     }

     strncpy(seedurl, argv[1], MAX_URL_LENGTH);
     snprintf(command, sizeof(command), "./wget.sh \"%s\"", seedurl);//creating wget command
     if(system(command) > 0)//if download fails seedurl is invalid
     {
	  printf("Error: invalid URL\n");
	  return 1;
     }
     
     path = (char *) malloc(strlen(argv[2])+2);
     MALLOC_CHECK(path);
     BZERO(path, strlen(argv[2])+1);

     strcpy(path, argv[2]);
     if(argv[2][strlen(argv[2])-1] != '/')//add '/' to the end of the path if it's not there
	  strncat(path, "/", sizeof(path));

     //validating path
     struct stat filestat;
     if(stat(path, &filestat) < 0)//checks whether path exists
     {
	  printf("error: invalid path\n");
	  return 1;
     }
     else if(!(S_ISDIR(filestat.st_mode)))//checks if path is a directory
     {
	  printf("error: inputted path is not a directory\n");
	  return 1;
     }


     maxdepth = atoi(argv[3]);

     if ((maxdepth < 0) || (maxdepth > 4))//check whether depth is between 1 and 4
     {
	  printf("Error: enter a maximum depth that is an integer between 1 and 4\n");
	  return 1;
     }
     
     Crawlparams *c = (Crawlparams *) malloc(sizeof(Crawlparams));
     c->path = path;
     c->maxdepth = maxdepth;
     c->dict = dict;
     
     bfcrawl(c,seedurl);
     saveDict("../data/webgraph.dat",dict);
     freeDict(&dict);
     free(c->path);
     free(c);
     return 0;
}
