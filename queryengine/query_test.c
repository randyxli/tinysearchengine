#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../util/dictionary.h"
#include "../util/header.h"

// Useful MACROS for controlling the unit tests.

// each test should start by setting the result count to zero

#define START_TEST_CASE  int rs=0

// check a condition and if false print the test condition failed
// e.g., SHOULD_BE(dict->start == NULL)

#define SHOULD_BE(x) if (!(x))  {rs=rs+1; \
    printf("Line %d Fails\n", __LINE__); \
  }

// return the result count at the end of a test

#define END_TEST_CASE return rs

//
// general macro for running a best
// e.g., RUN_TEST(TestDAdd1, "DAdd Test case 1");
// translates to:
// if (!TestDAdd1()) {
//     printf("Test %s passed\n","DAdd Test case 1");
// } else { 
//     printf("Test %s failed\n", "DAdd Test case 1");
//     cnt = cnt +1;
// }
//

#define RUN_TEST(x, y) if (!x()) {              \
    printf("Test %s passed\n", y);              \
} else {                                        \
    printf("Test %s failed\n", y);              \
    cnt = cnt + 1;                              \
}


#define CREATE_TEST_NODES()			\
DNODE *n1 = NULL;\
DNODE *n2 =  NULL;\
DNODE *n22 =  NULL;\
DNODE *n3 =  NULL;\
n1 = (DNODE *) malloc(sizeof(DNODE));\
n1->next = NULL;\
n1->depth = 1;\
strncpy(n1->key,"first",KEY_LENGTH);\
n1->data = NULL;\
\
n2 = (DNODE *) malloc(sizeof(DNODE));\
n2->next = NULL;\
n2->depth = 10;\
strncpy(n2->key,"second",KEY_LENGTH);\
n2->data = NULL;\
\
n22 = (DNODE *) malloc(sizeof(DNODE));\
n22->next = NULL;\
n22->depth = 10;\
strncpy(n22->key,"second",KEY_LENGTH);\
n22->data = NULL;\
\
n3 = (DNODE *) malloc(sizeof(DNODE));\
n3->next = NULL;\
n3->depth = 100;\
strncpy(n3->key,"third",KEY_LENGTH);\
n3->data = NULL;\


int AND1()
{
     START_TEST_CASE;
     CREATE_TEST_NODES();
     n1->next = n2;
     n22->next = n3;
     DNODE *result = AND(n1,n22);
     SHOULD_BE(!strcmp(result->key, "second"));
     SHOULD_BE(result->depth == n2->depth+n22->depth);
     SHOULD_BE(!result->next);
     free(n1);
     free(n2);
     free(n22);
     free(n3);
     END_TEST_CASE;
}

int OR1()
{
     START_TEST_CASE;
     CREATE_TEST_NODES();
     n1->next = n2;
     n22->next = n3;
     DNODE *result = OR(n1,n22);
     SHOULD_BE(!strcmp(result->key,"first"));
     SHOULD_BE(result->depth ==n1->depth);

     result = result->next;
     SHOULD_BE(!strcmp(result->key,"third"));
     SHOULD_BE(result->depth ==n3->depth); 

     result = result->next;
     SHOULD_BE(!strcmp(result->key,"second"));
     SHOULD_BE(result->depth == n2->depth+n22->depth);   
         
     free(n1);
     free(n2);
     free(n22);
     free(n3);
     END_TEST_CASE;
}

int main()
{
     int cnt = 0;

     RUN_TEST(AND1, "AND1");
     RUN_TEST(OR1, "OR1");
     if(!cnt)
     {
	  printf("All passed!\n");
	  return 0;
     }
     else
     {
	  printf("%d fails\n", cnt);
	  return 1;
     }
     return 0;
}
