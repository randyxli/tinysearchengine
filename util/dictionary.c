/*
File: dictionary.c

Programmer: Randy Li

Description: Contains all functions that manipulate DICTIONARY and related data structures.
 */
#include <stdlib.h>
#include "hash.h"
#include "header.h"
#include <string.h>
#include <stdio.h>
#include "dictionary.h"



/*
Description: Checks whether given key is already in the dictionary or not. Please see documentation on data structures for more complete understanding of the function.

Input: DICTIONARY *dict points to the dictionary, and char *newkey is the URL to be checked.

Output: If newkey was found in dict, returns a pointer to the DNODE where newkey was found. If newkey was not found in dict, then returns NULL.

*****PSEUDO CODE*****
(1) while(linked list still has elements), we search through the linked list for matches to newkey.
(2)    if(newkey=url in the linked list)
           return current node
       endif
(3)    go to next element in linked list
    endwhile
(4) return NULL
 */
DNODE *uniqueKey(DICTIONARY *dict, char *newkey)
{
     int hashedkey = hash1(newkey) % MAX_HASH_SLOT;//can improve speed     
     
     DNODE *node = dict->hash[hashedkey];//refers to nodes in the current hash slot of the dictionary
     
     //(1) node is only NULL when at the end of the linked list
     while(node != NULL)
     {
	  if(strcmp(node->key,newkey)==0)//(2) if we found the url in the dictionary return 1
	  {
	       return node;
	  }
	  node = node->next;//(3)
     }
     return NULL;
}

/*
Description: If the word to be added is not already in the DICTIONARY, creates two new DNODEs containing the required information and adds them into the DICTIONARY. Also keeps track of the number of times things have been inserted in the DICTIONARY. This function replaces function addToDict. Please see data structure documentation to see the differences with CS50's official solution.

Input: A pointer to a DICTIONARY, char *word is the key to be added in the first dimension of the linked list, and char *newurl is the key to be added in the second dimension of the linked list.

Output: 0 if the URL was added into the DICTIONARY, 1 if not.

Outputf: newunode, newdnode
 */
int addWordToDict(DICTIONARY *dict, char *word, char *newurl)
{     
     int hashedkey = hash1(word)%MAX_HASH_SLOT;
     DNODE *wordnode = NULL;
     if(!(wordnode = uniqueKey(dict, word)))//if word isn't in the dictionary
     {
	  wordnode = (DNODE *) calloc(1,sizeof(DNODE));
	  MALLOC_CHECK(wordnode);
	  
	  strncpy(wordnode->key, word, KEY_LENGTH);
	  wordnode->next = dict->hash[hashedkey];
	  wordnode->depth = 0;
	  wordnode->data = NULL;
	  dict->hash[hashedkey] = wordnode;
     }
     
     DNODE *docnode = NULL;
     if(!strcmp("http://www.cs.dartmouth.edu/site-content/site/phd-admissions.php",newurl) && !strcmp("send", word))
	docnode = NULL;
     if(!(docnode = searchLL(wordnode->data, newurl)))
     {
	  docnode = (DNODE *) malloc(sizeof(DNODE));
	  MALLOC_CHECK(docnode);

	  strncpy(docnode->key, newurl, KEY_LENGTH);
	  docnode->next = wordnode->data;
	  docnode->depth = 0;
     }
     wordnode->data = docnode;
     wordnode->depth++;
     docnode->depth++;
     return 0;
}


/*
Description: Frees all data structures associated with a DICTIONARY, including the DICTIONARY itself.

Input: A pointer to a pointer to a DICTIONARY.

Output: None.
 */
void freeDict(DICTIONARY **dict)
{
     for(int i=0; i<MAX_HASH_SLOT; i++)
     {
	  if((*dict)->hash[i]!=NULL)
	       freeNestedLL((*dict)->hash[i]);
     }
     free(*dict);
}

/*
Description: saveDict saves a DICTIONARY that holds 2-dimensional linked list to a text file. Each node is delimited by a special character, with a different special character for different linked list dimensions.

Input: char *filename is the full path to the destination file of the save.
DICTIONARY *dict a pointer to the DICTIONARY that will be saved.

Output: Returns nothing, but saves the data in dict into filename.

*****PSEUDO CODE*****
(1) Save number of hash slots to filename.
(2) for( 0<i>MAX_HASH_SLOT)
(3)    while(wordnode linked list still has unvisited nodes)
(4)        save current node to filename
           while(docnode linked list still has unvisited nodes)
(5)            save wordnode's docnodes to filename
       endwhile
    endfor
 */
void saveDict(char *filename, DICTIONARY *dict)
{
     DNODE *wordnode = NULL;
     DNODE *docnode = NULL;

     FILE *fp = fopen(filename, "a+");
     //(1)
     fprintf(fp, "%d\n", MAX_HASH_SLOT);
     for(int i=0; i<MAX_HASH_SLOT; i++)//(2)
     {
	  wordnode = dict->hash[i];
	  while(wordnode)//(3) continue if wordnode != NULL
	  {
	       char *nodetostring = DNODEToString(wordnode);
	       fprintf(fp,"#%s", nodetostring);//print wordnode to file
	       free(nodetostring);

	       docnode = wordnode->data;
	       while(docnode)//(4) continue if docnode != NULL
	       {
		    nodetostring = DNODEToString(docnode);
		    fprintf(fp, "@%s", nodetostring);//print docnode to file
		    free(nodetostring);
		    
		    docnode = docnode->next;
	       }     
	       wordnode = wordnode->next;	       
	  }
	  fprintf(fp, "\n");
     }
}

DICTIONARY *loadDict(char *datfile)
{
     FILE *fp = fopen(datfile, "r");
     int hashslots = 0;
     fscanf(fp, "%d\n", &hashslots);
     if(hashslots != MAX_HASH_SLOT)
     {
	  printf("Error: MAX_HASH_SLOT has changed since %s was created. Either re-run indexer or change MAX_HASH_SLOT to %d to resolve.\n", datfile, hashslots);
	  return NULL;
     }
     DICTIONARY *dict = (DICTIONARY *) calloc(1,sizeof(DICTIONARY));
     char c = fgetc(fp);
     char curword[KEY_LENGTH];
     char docname[KEY_LENGTH];
     int occurrences = 0;
     while(c!=EOF)
     {
	  if(c=='#')
	  {
	       fscanf(fp, "%*d %s", curword);
	       
	  }
	  if(c=='@')
	  {
	       fscanf(fp, "%d %s", &occurrences, docname);
	       for(int i=0; i<occurrences; i++)
		    addWordToDict(dict, curword, docname);
	  }
	  c = fgetc(fp);
     }
     fclose(fp);
     return dict;
}

/*
Description: Frees all data structures associated with a DICTIONARY, including the DICTIONARY itself.

Input: A pointer to a pointer to a DICTIONARY.

Output: None.
 
void freeDictCR(DICTIONARY **dict)
{
     for(int i=0; i<MAX_HASH_SLOT; i++)
     {
	  if((*dict)->hash[i]!=NULL)
	       freeLL((*dict)->hash[i]);
     }
     free(*dict);
}


*/
