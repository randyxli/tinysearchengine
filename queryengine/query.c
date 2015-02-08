/*
FILE: query.c

Input: On the command line, takes the path of an index file as a parameter. Prompts the user for an query and gives the result of the output on the command line.

Output: A list of URLs, ranked by word frequency.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include "../util/header.h"
#include "../util/dictionary.h"

#define MAX_OP_LENGTH strlen("and")

/*
Description: checks the precedence of an operator, or returns -1 if the input is not an operator. Originally written for extensibility, but now I realize that this paradigm of parsing is not extensible at all. I would be better off implementing a stack.

Input: char *op is a string that contains an operator (such as AND).

Output: The precedence of op. If it's not an operator, then return -1.
 */
int precedenceOf(char *op)
{
     if(!strcmp(op, "and"))
	  return 14;
     if(!strcmp(op, "or"))
	  return 13;
     return -1;
}


/*
Description: Retrieves all webpages related to the given query. Beware: contains very ugly control statements!

Input: char *query is the query the user inputted.

Output: The head of a DNODE linked list that contains all relevant documents.
*/ 
DNODE *processQuery(DICTIONARY *index, char *query)
{
     char splitquery[strlen(query)][strlen(query)];//contains all the individual tokens of the query, where a token is a word or an operator (AND or OR)
     int i=0;
     while(query[i])//convert all characters to lower case
     {
	  query[i] = tolower(query[i]);
	  i++;
     }


     char *token = strtok(query, " \n");
     int j = 0;
     int precedence = 1;
     while(token)//obtaining all the tokens from the query
     {
	  strcpy(splitquery[j],token);
	  if(precedence>0 && precedenceOf(token)>0)//detecting whether two operators are side by side
	  {
	       LOG("Illegal use of operators\n");
	       return NULL;
	  }
	  precedence = precedenceOf(token);
	  token = strtok(NULL, " \n");
	  j++;
     }
     
     
     DNODE *result = NULL;
     if(uniqueKey(index,splitquery[0]))//getting initial document list, if possible
	  result = copyLL((DNODE *)(uniqueKey(index,splitquery[0])->data));
     DNODE *temp = NULL;
     i=1;
     while(i<j)//the actual processing of a query
     {
	  DNODE *nexti = uniqueKey(index,splitquery[i+1]);
	  DNODE *nextnexti = uniqueKey(index,splitquery[i+3]);
	  if(precedenceOf(splitquery[i]) < 0)//case where two words are next to each other without an operator. Ex: randy li, which translates into randy AND li
	  {
	       temp = result;
	       if(uniqueKey(index,splitquery[i]))
	       {
		    result = AND(result, (DNODE *)(uniqueKey(index,splitquery[i])->data));
		    freeNestedLL(temp);
	       }
	       i++;
	  }
	  else if(!strcmp(splitquery[i], "or"))//case where the parser finds "OR" as the closest operator. It evaluates the expression immediately
	  {
	       temp = result;
	       if(nexti)
	       {
		    result = OR(result, (DNODE *)(nexti->data));
		    freeNestedLL(temp);
	       }
	       i = i+2;
	  }
	  else if(i<j-2)//case where the operator is AND, so we must check if the next operator is higher precedence.
	  {
	       if(!strcmp(splitquery[i+2],"or"))//if the next operator is of higher precedence, process that operation first
	       {
		    temp = result;
		    DNODE *temp1 = NULL;
		    if(nexti && nextnexti)
		    {
			 temp1 = OR((DNODE *)(nexti->data), (DNODE *)(nextnexti->data));
			 freeNestedLL(temp);
			 freeNestedLL(temp1);
		    }
		    else if(nexti)		    
			 temp1 = nexti->data;
		    else if(nextnexti)
			 temp1 = nextnexti->data;
		    else temp1 = NULL;
		    
		    result = AND(result,temp1);		    
		    i = i+4;
	       }
	  }
	  else//if not, just process the AND
	  {
	       temp = result;
	       if(nexti)
	       {
		    result = AND(result, (DNODE *)(nexti->data));
	            freeNestedLL(temp);
	       }
	       i = i+2;
	  }
	  
     }
     return result;
}

/*
Description: For each webpage, multiplies the query word frequency by the number of links pointing to the page to get a better ranking.

Input: DICIONARY *webgraph is the DICTIONARY produced by crawler.
DNODE *head is the head of the linked list that contains the results of the query to be ranked.

Output: None. The function simply modifies all the ranks of the webpages (represented by the depth member of the DNODE struct).
 */
void linkRank(DICTIONARY *webgraph, DNODE *head)
{
     DNODE *node = head;
     while(node)
     {
	  node->depth = (node->depth) * (uniqueKey(webgraph, node->key)->depth);
	  node = node->next;
     }
}

int main(int argc, char *argv[])
{
     if(argc != 2)
     {
	  printf("Error: invalid inputs. Please enter exactly one input.\n");
	  return 1;
     }

     char indexpath[strlen(argv[1])+1];
     strncpy(indexpath, argv[1], sizeof(indexpath));

     struct stat filestat;

     if(stat(indexpath, &filestat) < 0)//checking whether the given path is valid
     {
	  printf("Error: invalid index file.\n");
	  return 1;
     }

     char command[strlen(indexpath)+strlen("@in   ./tmp")];
     snprintf(command, sizeof(command), "@in %s ./tmp", indexpath);
     
     //rebuild the webgraph and inverted word list
     DICTIONARY *index = loadDict(indexpath);
     DICTIONARY *webgraph = loadDict("../data/webgraph.dat");

     while(1)
     {
	  //getting user's query
	  printf("Please enter your query. Type quit to quit:\n");
	  char query[1000];
	  fgets(query, sizeof(query), stdin);
	  if(!strcmp(query,"quit\n"))
	       break;

	  //processing the query
	  DNODE *results = processQuery(index, query);
	  linkRank(webgraph, results);
	  results = sortLL(results);
	  printq(results);
	  freeNestedLL(results);
     }
     freeDict(&index);
     freeDict(&webgraph);
     
     return 0;
}
