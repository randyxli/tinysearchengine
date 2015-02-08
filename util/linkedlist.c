

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "header.h"
#include "dictionary.h"

/*
Description: Prints the keys of all the DNODES in the queue in order.

Input: DNODE *q points to the start of the queue that is to be printed.

Output: Prints all the keys in the queue to the screen and the number of elements in the queue.
 */
void printq(DNODE *q)
{
     printf("Printing results\n");
     int i=0;
     while(q!=NULL)
     {
	  printf("%d %s\n", q->depth, q->key);
	  q = q->next;
	  i++;
     }
     printf("%d results\n", i);
}

/*
Description: Removes the leading element in a queue.

Input: DNODE **q points to a pointer that points to the leading element of a queue.

Output: None.
 */
void dequeue(DNODE **q)
{
     DNODE *temp = *q;
     *q = (*q)->next;
     free(temp);     
}


/*
Description: Frees all the memory locked up inside a two-dimensional linked list.

Input: DNODE *node points to an element in a DNODE linked list that is going to be freed.

Output: Nothing.
 */
void freeNestedLL(DNODE *node)
{
     DNODE *nested = NULL;
     while(node!=NULL)
     {      
	  nested = node->data;
	  while(nested!=NULL)
	       dequeue(&nested);
	  dequeue(&node);	  
     }
     return;
}

/*
Description: Reads all the non-pointer members of a DNODE into a string.

Input: DNODE *node is any non-null DNODE.

Output: A string in the following format: "%d %s ", where %d is node->depth and %s is node->key.

Outputf: char *str
 */
char *DNODEToString(DNODE *node)
{
     char *str = calloc(1, KEY_LENGTH+8*sizeof(int)+3);
     MALLOC_CHECK(str);

     sprintf(str, "%d %s ", node->depth, node->key);
     return str;
}

/*
Description: Searches a one-dimensional linked list for a key.

Input: DNODE *head is the head of the linked list to be searched.
char *newkey is the key to be searched for.

Output: If newkey is found in the linked list, returns the pointer to the DNODE where newkey was found. Otherwise, returns null.
 */
DNODE *searchLL(DNODE *head, char *newkey)
{
     DNODE *node = head;
     while(node != NULL)
     {
	  if(strcmp(node->key,newkey)==0)//(2) if we found the url in the dictionary return the node
	  {
	       return node;
	  }
	  node = node->next;//(3)
     }
     return NULL;
}

DNODE *copyLL(DNODE *head)
{
     DNODE *node = head;
     DNODE *copy = NULL;
     while(node)
     {
	  DNODE *newnode = (DNODE *) malloc(sizeof(DNODE));
	  newnode->next = copy;
	  newnode->data = node->data;
	  newnode->depth = node->depth;
	  strncpy(newnode->key, node->key, KEY_LENGTH);
	  copy = newnode;
	  node = node->next;
     }

     return copy;
}

DNODE *sortLL(DNODE *head)
{
     if(!head)
	  return NULL;
     DNODE *node = head;
     int size = 0;
     while(node)
     {
	  size++;
	  node = node->next;
     }

     node = head;
     DNODE *arr[size];
     for(int i=0; i<size; i++)
     {
	  arr[i] = node;
	  node = node->next;
     }

     int sorted = 0;
     while(!sorted)
     {
	  sorted = 1;
	  for(int i=0; i<size-1; i++)
	  {
	       if(arr[i]->depth < arr[i+1]->depth)
	       {
		    sorted = 0;
		    DNODE *temp = arr[i];
		    arr[i] = arr[i+1];
		    arr[i+1] = temp;
	       }
	  }
     }

     for(int i=0; i<size-1; i++)
     {
	  arr[i]->next = arr[i+1];
     }
     arr[size-1]->next = NULL;
     return arr[0];
}

DNODE *AND(DNODE *operand1, DNODE *operand2)
{
     if(!(operand1 && operand2))//if one of the lists is NULL, return NULL
	  return NULL;
     DNODE *result = NULL;
     DNODE *node = operand1;
     DNODE *node1 = NULL;

     while(node)
     {
	  node1 = searchLL(operand2, node->key);
	  if(node1)
	  {
	       DNODE *newnode = (DNODE *) malloc(sizeof(DNODE));
	       newnode->next = result;
	       newnode->depth = node1->depth + node->depth;
	       strncpy(newnode->key,node->key,KEY_LENGTH);
	       newnode->data = node->data;
	       result = newnode;
	  }
	  node = node->next;
     }
     return result;
}

DNODE *OR(DNODE *operand1, DNODE *operand2)
{
     if(!operand2 && !operand1)
	  return NULL;
     if(!operand2)
	  return operand1;
     if(!operand1)
	  return operand2;
     DNODE *result = copyLL(operand2);
     DNODE *node = operand1;
     DNODE *node1 = NULL;

     while(node)
     {
	  node1 = searchLL(result, node->key);
	  if(node1)
	  {	       
	       node1->depth = node1->depth + node->depth;
	  }
	  else
	  {
	       DNODE *newnode = (DNODE *) malloc(sizeof(DNODE));
	       newnode->next = result;
	       newnode->depth = node->depth;
	       strncpy(newnode->key,node->key,KEY_LENGTH);
	       newnode->data = node->data;
	       result = newnode;
	  }
	  node = node->next;
     }
     return result;
}
