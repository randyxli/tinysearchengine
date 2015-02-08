/*
Programmer: Randy Li

File: files.c

Description: Contains functions that process files.
 */

#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "dictionary.h"
/*
Description: appendToFile appends a string to a file.

Input: char *file is the full path to the file that will be appended to.
char* str is the string that will be appended to file.

Output: None.
 */
void appendToFile(char *file, char *str)
{
     FILE *fp = fopen(file, "a+");
     fprintf(fp, "%s", str);
     fclose(fp);
}

/*
Description: getFileContents reads a file into a string.

Input: char *filepath is the full path to the file that is to be converted into a string

Output: char *contents, a pointer to a string containing the contents of filepath.

Outputf: char *contents

*****PSEUDO CODE*****
(1) Find size of file filepath
(2) Read filepath into a string
(3) return 
 */
char *getFileContents(char *filepath)
{
     //(1) Using fseek to find file size
     int currentsize = 0;
     FILE *fp = fopen(filepath, "r");
     fseek(fp, 0, SEEK_END);
     currentsize = ftell(fp);
     fseek(fp,0, SEEK_SET);
     
     //(2) read filepath into contents using fread
     char *contents = (char *) malloc(sizeof(char)*currentsize+1);
     MALLOC_CHECK(contents);
     fread(contents, 1, currentsize, fp);
     fclose(fp);
     contents[currentsize] = '\0';//null-terminate contents
     return contents;
}
