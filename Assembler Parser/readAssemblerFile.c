#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "readAssemblerFile.h"
#include "instruction.h"

int fileLineNumber;
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function gets a file path and send every line to the analyse line function
 */
void readAssembler( char *fileName)
{
    FILE * fp = fopen(fileName,"r");

    if(!fp)
    {
        printf("error : cannot open file\n");
        exit(1);
    }

    char singleLine[80];
    while(!feof(fp))
    {
        fgets(singleLine,80,fp);
        fileLineNumber++;

        if(!checkEmptyLine(singleLine) && !checkCommentLine(singleLine)) /* ignore comment or empty lines */
        {
            analyseLine(singleLine);
        }
    }
    fclose(fp);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 checks if line is empty
 */
int checkEmptyLine(char * line)
{
    int i;

    for(i =0 ;  line[i] != '\n' && line[i] != '\0'; i++)
    {
        if(!isspace(line[i]))
            return FALSE;
    }
    return TRUE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 checks if line start with ';' , comment line
 */
int checkCommentLine(char * line)
{
    int i  = 0 ;
    while(line[i] != '\n' && line[i] != '\0')
    {
        if(isspace(line[i]))
            i++;

        if(line[i] == ';')
            return TRUE;

        if(!isspace(line[i]))
            return FALSE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------------------------------------------------*/

