#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "labels.h"
#include "instruction.h"
#include "memFunctions.h"
#include "opcodes.h"
#include "stringFunctions.h"

#define MAX_LEN_LABEL 31

aOperator aOperators[NUMBER_OF_OPERATORS];

int labelSize = 0;
symbolsTable * labels;

int fileLineNumber;

int isFoundError;
int fileLineNumber;

entry * entryListHead = NULL;
entry * entryListTail = NULL;

externData * externListHead = NULL;
externData * externListTail = NULL;


int DC;
int IC;
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function gets a label , type (directive or instruction) and address and update in a lists
 */
void updateLabel(char * newLabelName,  int type, int address)
{
    labelSize++;

    if(labelSize == 1) /* if the first label , malloc */
    {
        labels = mallocate(sizeof(symbolsTable));
    }
    else /* if addition label realloc the list */
    {
      labels = reallocate(labels,labelSize* sizeof(symbolsTable));
    }

    if(type == labelType_EXTERN) /* is Extern */
    {
        labels[labelSize-1].labelName = (char *)stringDup(newLabelName);
        labels[labelSize-1].adress = address;
        labels[labelSize-1].typ = labelType_EXTERN;
    }
    else
    {
        if (type == labelType_DIRECTIVE) /* is Directive */
        {
            labels[labelSize-1].labelName = (char *)stringDup(newLabelName);
            labels[labelSize-1].adress = address;
            labels[labelSize-1].typ = labelType_DIRECTIVE;
        }
        else /* is Instruction */
        {
            labels[labelSize-1].labelName = (char *)stringDup(newLabelName);
            labels[labelSize-1].adress = address;
            labels[labelSize-1].typ = labelType_INSTRUCTION;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function checks if the given label is valid
 */
int isValidLabel(char *potentialNewLabel)
{
    int i;
    unsigned int labelNameLength = strlen(potentialNewLabel);
    if(labelNameLength > MAX_LEN_LABEL)
    { /* checks that the label is no longer than 31 chars */
        printf("error : line %d) label name is too long, exceed by %d chars\n"
                ,fileLineNumber,(labelNameLength - MAX_LEN_LABEL));
        isFoundError = 1;
        return FALSE;
    }
    /*checks the label is not one of the directive's commands */
    if(strcmp(potentialNewLabel,".string") == 0 || strcmp(potentialNewLabel,".data") == 0 ||
            strcmp(potentialNewLabel,".extern") == 0 || strcmp(potentialNewLabel,".entry") == 0)
    {
        return FALSE;
    }
    /*checks the label is not one of the instruction's commands */
    for(i = 0 ; i < NUMBER_OF_OPERATORS ; i++)
    {
        if(strcmp(potentialNewLabel,aOperators[i].opName) == 0)
        {
             return FALSE;
        }

    }

    for(i = 0  ; potentialNewLabel[i] != '\0' ; i++)
    {
        if(i == 0 )
        { /* the label must start with and alpha */
            if(!isalpha(potentialNewLabel[i]))
            {

                printf("error : line %d) label name start with illegal char (%c)\n"
                        ,fileLineNumber,potentialNewLabel[i]);
                isFoundError = 1;
                return FALSE;
            }

        }
        else
        { /* labels must contain digits or alphas */
            if(isspace(potentialNewLabel[i]) ||
                (!isdigit(potentialNewLabel[i]) && !isalpha(potentialNewLabel[i])))
            {

                printf("error : line %d) illegal char in label name (%c)\n",fileLineNumber,potentialNewLabel[i]);
                isFoundError = 1;
                return FALSE;
            }
        }
    }
    return TRUE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function checks if label is already exist in the list
 */
int isLabelExist(char * labelName)
{
    int i;

    for(i = 0 ; i < labelSize ; i++ )
        if(strcmp(labelName,labels[i].labelName) == 0)
        {
            return TRUE;
        }
    return FALSE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function checks if there is label in a potential label in the start of given line
 */
char* checkForLabel(char * line)
{
    int i;
    char * pointsPos = strchr(line,':'); /* label must end with ':' */
    char* label;

    if(pointsPos) /* if found ':' */
    {
        label = mallocate((pointsPos - line)+1); /* the size of the string (label) until '"' */

        for(i = 0 ; line[i] != ':'; i++)
        {
            label[i] = line[i]; /* copy the potential label */
        }

        label[i] = '\0'; /* add end of string */
        return label;

    }
    else
    {
        return NULL;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function update an entry label in a struct and add it to the entry's  list
 */
void updateEntry(char * labelName)
{
    entry * newEntry = mallocate(sizeof(entry));
    newEntry->next = NULL;
    newEntry->labelName = (char *)stringDup(labelName);
    newEntry->adress = 0 ;
    newEntry->line = fileLineNumber;

    if(!entryListHead) /* if list is empty */
    {
        entryListHead = newEntry;
        entryListTail = newEntry;
    }
    else
    {
        entryListTail->next = newEntry;
        entryListTail = newEntry;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function returns  the label address
 */
int getLabelAdress(char *labelName)
{
    int i;

    for(i = 0 ; i < labelSize ; i++ )
        if(strcmp(labelName,labels[i].labelName) == 0)
        {
            return labels[i].adress;
        }
    return -1; /* -1 was not find the label in the list */
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function returns the label type
 */
int getLabelType(char * labelName)
{
    int i;
    for(i = 0 ; i< labelSize ; i++)
    {
        if(strcmp(labelName,labels[i].labelName) == 0)
        {
            return labels[i].typ;
        }
    }
    return -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function is looking for extern labels  to create extern list
 */
void buildExternList()
{
    int i;
    externData * extData = NULL;

    for(i = 0 ; i < labelSize ; i++)
    {
        if(labels[i].typ == labelType_EXTERN)
        {
            if(!isExternCreated(labels[i].labelName)) /* if extern found for the first time */
            {
                extData =createNewExtern(labels[i].labelName);
            }
            else /* if extern name already found */
            {
                extData = getExternData(labels[i].labelName);
            }
            fillExternUsedAdresses(extData);
            addExternDataToList(extData);
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function add extern node to the list
 */
void addExternDataToList(externData * extDataToAdd)
{
    if(externListHead == NULL) /* if list is empty */
    {
        externListHead = extDataToAdd;
        externListTail = extDataToAdd;
    }
    else
    {
        externListTail->next = extDataToAdd;
        externListTail = extDataToAdd;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function checks if extern label is already has been updated before in the list
 */
int isExternCreated(char * externLabelName)
{
externData * curExtData = NULL;
    if(externListHead == NULL)
    {
        return FALSE;
    }
    curExtData = externListHead;
    while(curExtData)
    {
      if(strcmp (curExtData->labelName,externLabelName) == 0)
      {
          return TRUE;
      }
      curExtData = curExtData->next;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 create a new node of extern data
 */
externData * createNewExtern(char * externLabelName)
{
    externData * extData = mallocate(sizeof(externData));
    extData->labelName = (char *)stringDup(externLabelName);
    extData->next = NULL;
    extData->size = 0;
    return extData;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 returns the extern data node of a given extern label name
 */
externData * getExternData(char * externLabelName)
{
    externData * curExtData = externListHead;
    while(curExtData)
    {
        if(strcmp(curExtData->labelName,externLabelName) == 0)
            return curExtData;

        curExtData = curExtData->next;
    }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
  this function looks for uses of the given extern label name in the code and fill
  an array of ints an addressess of every use of it
 */
void fillExternUsedAdresses(externData * curExtData)
{
    lineData * curLineData = head;
    insLine * curInsLine = NULL;
    int numOps , i;

    while(curLineData)
    {
        if(curLineData->lineTyp == lineType_INSTRUCTION)
        { /* extern label can be use only with instruction type line as an operand */
            curInsLine = (insLine *) curLineData->data;
            numOps = curInsLine->operator->numOfOperands;
            for( i = 0 ; i < numOps ; i++)
            {
                /* if this label has been used here as an operand */
                if(strcmp(curInsLine->dataOPs[i]->operand,curExtData->labelName) == 0)
                {
                    curExtData->size++;
                    if(curExtData->size == 1) /* malloc new array of ints */
                    {
                        /* update address */
                        curExtData->adressesItHasBeenUsed = mallocate(curExtData->size*sizeof(int *));
                    }
                    else /* realloc the arr of ints */
                    {
                        curExtData->adressesItHasBeenUsed =
                                reallocate(curExtData->adressesItHasBeenUsed,curExtData->size*sizeof(int *));
                    }
                    /* update address */
                    curExtData->adressesItHasBeenUsed[curExtData->size -1] = curInsLine->icAdress + i + 1;
                }
            }
        }
        curLineData = curLineData->next;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function going through the entry list and try to find the address which is where is was declared in the code
 */
void fixEntryAdress()
{
    entry * current = entryListHead;
    int newLabelAdress;

    while(current) /* looking for entry label that was found at the first pass */
    {
        newLabelAdress = getLabelAdress(current->labelName);
        if(newLabelAdress) /* if entry label was found */
        {
            newLabelAdress = newLabelAdress < 100 ? newLabelAdress +IC : newLabelAdress;
            current->adress = newLabelAdress; /* update its address */
        }
        else /* if label was not found */
        {
            printf("error : line %d) label doesn't exist (%s)\n",current->line,current->labelName);
            isFoundError = 1;
        }
        current = current->next;
    }
}
