#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stringFunctions.h"
#include "memFunctions.h"
#include "instruction.h"

#define IS_COMMA(c) ((c) == ',')
#define IS_WHITE_CHAR(c) ((c) == '\t' || (c) == ' ' )

/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function cutting duplicated white spaces, for example
the given string : "Hello                    world"
the returned pointer to a new string  will be "Hello world"
*/
char * cutDupWhiteSpaces(char * oldString)
{
    char * newString;
    int i = 0,j =0 ,spaceAtEnd = 0, endOfString = strlen(oldString) -1;

    while(isspace(oldString[endOfString]) && endOfString > -1)
    {
        spaceAtEnd++;
        endOfString--;
    }

    newString = mallocate(strlen(oldString)+1);

    while (IS_WHITE_CHAR(oldString[i]) && !IS_END_OF_STRING(oldString[i]))
    {
        i++;
    }

    while(i < strlen(oldString) - spaceAtEnd)
    {
        newString[j] = oldString[i];
        while(IS_WHITE_CHAR(oldString[i]) && IS_WHITE_CHAR(oldString[i + 1]) )
        {
            i++;
        }
        i++;
        j++;
    }
    newString = reallocate(newString,j+1);
    newString[j] = '\0';
    return newString;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function extracting out the first work in the string
for example, the given string is " Hello world"
    the returned pointer to a string will be "Hello"
*/
char  *getFirstWord (char  *givenString)
{
    char * firstWord;
    firstWord = mallocate(strlen(givenString)+1);
    int i=0,j= 0 ;

    while (IS_WHITE_CHAR(givenString[i]))
    {
        i++;
    }

    while (!IS_WHITE_CHAR(givenString[i]) && !IS_END_OF_STRING(givenString[i]))
    {
        firstWord[j] = givenString[i];
        i++;
        j++;
    }
    firstWord[j] = '\0';
    firstWord =reallocate(firstWord, j+1);
    
    return firstWord;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function delete all the white spaces in a given string
for example : the given string is : "     abc   ,   123     "
the returned pointer to a string will be "abc,123"
*/
char * delAllWhilteSpaces(char * oldString)
{
    char * newString;
    newString = mallocate(strlen(oldString)+1);
    int i,j;
    for (i = 0 , j = 0 ; i < strlen(oldString) ;j++,i++ )
    {
        while (IS_WHITE_CHAR(oldString[i]))
        {
            i++;
        }
        newString[j] = oldString[i];
    }
    newString[j] = '\0';
    return newString;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function returns the given string as a new one but without the first word
*/
char * removeFirstWord(char * oldString)
{
    char * newString;
    int i=0,j=0;
    newString = mallocate(strlen(oldString)+1);

    while (IS_WHITE_CHAR(oldString[i]))
    {
        i++;
    }
    while (!IS_WHITE_CHAR(oldString[i]) && !IS_END_OF_STRING(oldString[i]))
    {
        i++;
    }
    while (IS_WHITE_CHAR(oldString[i]))
    {
        i++;
    }
    while ( !IS_END_OF_STRING(oldString[i]))
    {
        newString[j++] = oldString[i++];
    }
    newString = reallocate(newString,strlen(newString)+1);
    newString[j] = '\0';
    return newString;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function split a given string by certain char that it gets and return a 2d arrays of its splitted  string
example : given string is " abc,123"
given char is ','
the return strings are [0] = "abc"
                       [1] = "123"
*/
char ** splitStringByChar(char* oldString, char whereToSplit[])
{
    char ** splittedStrings;
    char *stringToWorkWith , *token;
    int sizeOf2dArray = 1;

    stringToWorkWith = stringDup(oldString);

    token = strtok(stringToWorkWith,whereToSplit);

    if(token == NULL)
    {
        return NULL;
    }

    splittedStrings = mallocate((sizeOf2dArray+1) * sizeof(char*));
     splittedStrings[sizeOf2dArray-1] = delAllWhilteSpaces(token);

    token = strtok(NULL,whereToSplit);
    while (token != NULL)
    {
        sizeOf2dArray++;
        splittedStrings = reallocate(splittedStrings,(sizeOf2dArray + 1) * sizeof(char*));
        splittedStrings[sizeOf2dArray-1] = delAllWhilteSpaces(token);
        token = strtok(NULL,whereToSplit);
    }
    splittedStrings[sizeOf2dArray] = NULL;
    return splittedStrings;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function returns the size of a 2d splitted array
 */
int sizeOfSplittedString(char ** splittedString)
{
    int size;
    for (size = 0 ; splittedString[size] != NULL ; size++)
    {
        ;
    }
    return size;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function checks the validation of the commas in the string
it return false if there are , in the start of the string
, in the end of the string
duplicated ,,
if there are no , at all
otherwise it returns true
*/
int validNumOfCommas(char * givenString)
{
    int i = 0,isThereComma = 0, lastCharWasComma = 0;

    if(IS_COMMA(givenString[0])) /* comma in the beginning */
    {
        return FALSE;
    }
    if(IS_COMMA(givenString[strlen(givenString)-1])) /* comma in the end */
    {
        return FALSE;
    }
       while(!IS_END_OF_STRING(givenString[i]))   /* check for duplicated commas */
        {
           if(isspace(givenString[i]))
           {
               i++;
           }
           if(IS_COMMA(givenString[i]))
           {
               if(lastCharWasComma)
               {
                   return FALSE;
               }
               lastCharWasComma =  1;
           }
           if(!isspace(givenString[i]) && !IS_COMMA(givenString[i]))
           {
               lastCharWasComma = 0 ;
           }
           i++;
        }
    for(i = 0 ; givenString[i] != '\0' ; i++)
    {
        if(IS_COMMA(givenString[i]))
        {
            isThereComma = 1;
        }
    }
    return isThereComma;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function free a 2d splitted array
 */
void free2dArray(char **arrToFree, int size)
{
    int i;
    for(i = 0; i < size ; i ++ )
    {
        free(arrToFree[i]);
    }

    free(arrToFree);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function returns the string between the quotation marks
 example : "the string "abc" is the string to return"
 returns "abc" (without the quotation marks obviously)
 */
char *getBetweenQuotationMarks(char * line)
{
    int i = 0 , qCounter =0; ;
    char * startQ = NULL, * endQ = NULL, *inBetween;
    while (!IS_END_OF_STRING(line[i]))
    {
        if(line[i] == '"')
        {
            qCounter++;
            if(qCounter > 2)
                return NULL;

            if(startQ)
                endQ = &line[i];
            else
                startQ = &line[i];
        }
        i++;
    }
    if(startQ && endQ)
    {
        i = 0;
        char * writeFromHere;
        inBetween = mallocate(strlen(line)+1);
        for(writeFromHere = (startQ+1) ; writeFromHere != (endQ) ; writeFromHere++)
        {
            inBetween[i] = writeFromHere[0];
            i++;
        }
        inBetween[i] = '\0';

        return reallocate(inBetween,strlen(inBetween)+1);
    }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function is like strdup()
 I wrote it because c90 doesn't allow to use strdup()
 */
char * stringDup(char * strToDuplicate)
{
    int i ;
    unsigned int  size = strlen(strToDuplicate);
    char * duplicatedString;
    duplicatedString = mallocate(size+1);
    for( i = 0 ; i < size ; i++ )
    {
        duplicatedString[i] = strToDuplicate[i];
    }
    duplicatedString[i] = '\0';
    return duplicatedString;
}



