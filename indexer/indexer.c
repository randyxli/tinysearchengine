#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "../util/header.h"
#include "../util/html.h"
#include "../util/crawler.h"
#include "../util/dictionary.h"

#define FILECOUNT "./filecount.tmp"
#define INDEX "../data/index.dat"
#define WDELIM 30
#define DDELIM 31


/*
Description: parse parses a webpage for words, adding each word into a DICTIONARY.

Input: DICTIONARY *dict is a pointer to a DICTIONARY that will hold the inverted list.
char *page is the string form of a webpage.

Output: Returns nothing.

Outputf: All the new wordnodes and docnodes added.

*****PSEUDO CODE*****
(1) while(end of page has not been reached)
(2)      getNextWord from page
(3)      add the new word to the dictionary
    endwhile
 */
void parse(DICTIONARY *dict, char* page)
{
     char url[MAX_URL_LENGTH];
     sscanf(page, "%s\n", url);//get page's URL from the first line of page
     int pos=0;
     
     char word[MAX_URL_LENGTH];
     BZERO(word, MAX_URL_LENGTH);
     
     //(1)
     while((pos = getNextWord(pos, page, word)) > 0)//(2) getNextWord returns -1 when it's done parsing. pos is the current index of page that it's parsing.
     {	  
	  addWordToDict(dict, word, url);//(3)adds both a wordnode containing word and a documentnode containing url
	  BZERO(word, sizeof(word));
     }
}


/*
Description: Indexes a repository of webpages into an inverted list. The inverted list is saved into a .dat file after completion.

Input: char *path is the full path to the directory containing the webpages to be indexed.

Output: Returns nothing. Saves a .dat file with all the data from the inverted list.

*****PSEUDO CODE*****
(1) Count the number of html files inside path.
(2) for(each page in path)
(3)     get contents of a page
(4)     parse the page and add each word to the inverted word list
    endfor
(5) save inverted word list
 */
void indexer(char *path)
{
     char *command = (char *) malloc(strlen(path)+strlen(FILECOUNT)+strlen("ls %s | grep .html | wc -l > %s"));
     MALLOC_CHECK(command);
     
     //(1) Count the number of html files inside path.
     sprintf(command, "ls %s | grep .html | wc -l > %s", path, FILECOUNT);//form bash shell command
     if(system(command)!=0)//execute command
     {
	  LOG("Error: shell returned non-zero exit status");
	  return;
     }

     //obtaining html file count from temporary file
     char *temp = getFileContents(FILECOUNT);
     char file[strlen(temp)+strlen(path)+strlen(".html")+1];
     int filecount = atoi(temp);
     free(temp);

     DICTIONARY *dict = (DICTIONARY *) calloc(1,sizeof(DICTIONARY));
     MALLOC_CHECK(dict);
     char *page = NULL;
     
     
     for(int i=0; i<filecount; i++)//(2)
     {
	  snprintf(file, sizeof(file), "%s%d.html", path, i);
	  
	  page = getFileContents(file);//(3)
	  parse(dict, page);//(4)
	  free(page);	  
     }

     saveDict(INDEX, dict);//(5)
     free(command);
     freeDict(&dict);
}

/*
Description: Depending on the arguments, indexes webpages or rebuilds an index from .dat files.

Input: The inputs are given on the command line and follow one of two forms:
[EXECUTABLE] [WEBPAGE REPO]
or
[EXECUTABLE] [INDEX.DAT] [REBUILT_INDEX.DAT]
where EXECUTABLE is the executable for this program, WEBPAGE REPO is a full path to a directory containing webpages downloaded by crawler, INDEX.DAT is the full path to an index file that was created by this indexer, and REBUILT_INDEX.DAT is the full path to the file that contains the rebuilt and then saved INDEX.DAT.

Output: 0 if program ran successfully, nonzero if not.
 */
int main(int argc, char* argv[])
{
     
     if(argc != 2 && argc != 3)
     {
	  printf("Error: must have exactly 1 or 2 arguments\n");
	  return 1;
     }

     char *path = (char *) malloc(strlen(argv[1])+1);
     MALLOC_CHECK(path);
     strcpy(path,argv[1]);
     struct stat filestat;

     if(stat(path, &filestat) < 0)
     {
	  printf("Error: invalid path\n");
	  return 1;
     }
     
     if(S_ISDIR(filestat.st_mode) && (argc==2))
     {
	  if(path[strlen(path)-1] != '/')//add '/' to the end of the path if it's not there
	       strncat(path, "/", sizeof(path));
	  indexer(path);
     }
     else if (!strcmp(path+strlen(path)-4, ".dat") && (argc==3))
     {
	  char newdat[strlen(argv[2])+1];
	  strcpy(newdat, argv[2]);
	  char command[strlen("touch     ")+strlen(newdat)];
	  snprintf(command, sizeof(command), "touch %s", newdat);	  
	  if(system(command) != 0)
	  {
	       printf("Error: %s is an invalid path. Please enter a valid one.\n",newdat);
	       return 1;
	  }
	  DICTIONARY *dict = loadDict(path);
	  if(!dict)
	       return 1;
	  else
	  {
	       saveDict(newdat, dict);
	       freeDict(&dict);
	  }
     }
     else 
     {
	  printf("error: invalid path. First argument must be a directory or a .dat file\n");
	  return 1;
     }
     
     free(path);
     return 0;
}
