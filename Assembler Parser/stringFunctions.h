#ifndef ASSEMBLER_STRINGFUNCTIONS_H
#define ASSEMBLER_STRINGFUNCTIONS_H

#define IS_END_OF_STRING(c)  ((c) == '\0')

char * cutDupWhiteSpaces(char * );
char  *getFirstWord (char  *);
char * delAllWhilteSpaces(char * );
char * removeFirstWord(char * );
char ** splitStringByChar(char* , char[]);
int sizeOfSplittedString(char ** );
int validNumOfCommas(char * );
void free2dArray(char **, int);
char *getBetweenQuotationMarks(char * );
char * stringDup(char * );



#endif /* ASSEMBLER_STRINGFUNCTIONS_H */
