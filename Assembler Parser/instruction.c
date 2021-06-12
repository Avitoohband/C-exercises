#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "instruction.h"
#include "opcodes.h"
#include "stringFunctions.h"
#include "memFunctions.h"
#include "labels.h"


#define COMMA ","
#define IS_SIGNED(c) ((c) == '-' || (c) == '+')
#define NUM_OF_AM 4
#define VALID_REG_NUM(c) ((c) >= '0' && (c) <= '7')

int isFoundError = 0;
int fileLineNumber;

int labelSize;
symbolsTable * labels;

entry * entryListHead;

externData * externListHead;



int IC = 100;
int DC = 0;
lineData *head = NULL;
lineData *tail = NULL;

aOperator aOperators[NUMBER_OF_OPERATORS];
/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function get the name of the instruction and returns the struct
 of the opcode
*/
aOperator * getStruct(char * nameOfInstruction)
{
    int i ;
    for(i = 0 ; i < NUMBER_OF_OPERATORS ; i++)
    {
        if(strcmp (nameOfInstruction ,aOperators[i].opName) == 0)
            return &aOperators[i];
    }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function checks if there are only digits in the string
 returns true if it does.
 */
int isDigits(char * potentialOperand)
{
    int i;

     if(!potentialOperand[0]) /* if string is NULL*/
     {
         printf("Null in operand\n");
         return FALSE;
     }

    for (i = 0 ; !IS_END_OF_STRING(potentialOperand[i]) ; i++)
     {
         if(i == 0)
         {/* check if first char is digts or a signed*/
             if(!isdigit(potentialOperand[i]) && !IS_SIGNED(potentialOperand[i]))
             {
                 printf("%c is not an operand or signed\n",potentialOperand[i]);
                 return FALSE;
             }

         }
         else
         {/* check if digits only*/
             if(!isdigit(potentialOperand[i]))
             {
                 printf("%c is not an operand\n",potentialOperand[i]);
                 return FALSE;
             }

         }
    }
    return TRUE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 assist function*
 this function checks if the line is a valid name of register
 */
int isReg(char * line)
{
    int foundInvalidChar = 0 ;
    int i=2;
    if (strlen(line) < 2)
    {
        return FALSE;
    }
    if(line[2] != '\0') /* if string is \0 in cell 2 its len is 3 so it can be name of reg*/
    while(line[i] != '\0') /* if string doesn't end at the 3rd cell, if not space returns false*/
    {
        if(!isspace(line[i]))
            foundInvalidChar = 1;
        i++;
    }

    if(line[0] == 'r' && VALID_REG_NUM(line[1]) && !foundInvalidChar) /*/ check if valid name of reg*/
        return TRUE;

    return FALSE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 assist function*
 this function checks if the source or destination has the current addressing method
 */
int isHaveAM(adressingMethods * addMethods, adressingMethods AM)
{
    int i , isHave = 0;
    for (i = 0; i < NUM_OF_AM; i++) {
        if (AM == addMethods[i])
            isHave = 1;
    }
    return isHave;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function analyse the line, if it have label at first then if its directive or instruction.
*/
void analyseLine (char * oldLine)
{
    int originalInsCounter = IC;
    int originalDataCounter = DC;
    int isLabel = 0 ; /* if label is good or not*/
    char * lineSingleWhiteSpaces, *firstWord,*label ,*newLine,*lineWithoutFirstWord;
    aOperator *data;

    label = checkForLabel(oldLine); /* check for a potential label at the start of the string*/


    if(label) /* check if there is label*/
    {
        if(isValidLabel(label))
        {
            if(!isLabelExist(label))
            {
                isLabel = TRUE;
            }
            else
            {
                printf("error : line %d) label is already exist (%s)\n",fileLineNumber,label);
                isFoundError = 1;

            }
        }
        else
        {
            printf("error : line %d) label is not valid (%s)\n",fileLineNumber,label);
            isFoundError = 1;

        }
        newLine = removeFirstWord(oldLine); /*if label >  remove the label from the string*/
    }
    else
    {
        newLine = oldLine;/* if no label > leave how it is*/
    }

    lineSingleWhiteSpaces = cutDupWhiteSpaces(newLine); /* take off duplicated spaces*/
    firstWord = getFirstWord(lineSingleWhiteSpaces); /* get the first word to a different pointer*/

    data = getStruct(firstWord); /* check if the word is one of the operators*/
    if(data) /* checks what instruction*/

    {
        lineWithoutFirstWord = removeFirstWord(lineSingleWhiteSpaces);
        checkInstruction(lineWithoutFirstWord, data);

        if(isLabel) /* if label is valid*/
        {
            updateLabel(label,labelType_INSTRUCTION, originalInsCounter);
        }

        free(lineSingleWhiteSpaces);
    }
    else /* check for directive*/
    {
        lineWithoutFirstWord = removeFirstWord(lineSingleWhiteSpaces);
        if(isLabel)
        checkDirective(firstWord, lineWithoutFirstWord,label,originalDataCounter);
        else
           checkDirective(firstWord, lineWithoutFirstWord,NULL,originalDataCounter);
    }
    free(label);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function checks the instruction and if the operand\s are valid
 */
void checkInstruction(char *line, aOperator *operator)
{
    char **operands;
    int numOps,i;
   /* int operatorNumber = operator - &aOperators[0];*/
    insLine *insLin; /* instruction line*/
    location loc;

    insLin = mallocate(sizeof(insLine));
    insLin->operator = operator;
    insLin->icAdress = IC;

    if (operator->numOfOperands == 2) /* there should be 2 operands*/
    {
        if (!validNumOfCommas(line))
        {
            printf("error : line %d) illegal number of commas\n",fileLineNumber);
            isFoundError = 1;
            free(insLin);
            return;
        }
        operands = splitStringByChar(line, COMMA); /* split the operands to 2d array*/
        numOps = sizeOfSplittedString(operands); /* size of 2d array*/

        if (numOps == operator->numOfOperands) /* checks if num of operands similar to what it should be
            example : if "mov" , so it should be 2 operands*/
        {
            loc = SOURCE;

            for (i = 0; i < numOps; i++)
            {
                insLin->dataOPs[i] = checkAndProccessAM(operands[i], operator, loc);
                loc = DEST;
            }
            /*if both operators are registers count as one "Instruction Count"*/
            if (numOps == 2 && insLin->dataOPs[1] != NULL &&
                (insLin->dataOPs[0]->addMethod == REG_DIRECT ||
                 insLin->dataOPs[0]->addMethod == REG_INDIRECT) &&
                (insLin->dataOPs[1]->addMethod == REG_DIRECT ||
                 insLin->dataOPs[1]->addMethod == REG_INDIRECT))
            {
                IC--;
            }
            addLineData(insLin, lineType_INSTRUCTION);
            IC += 1 + numOps;
        }
        else
        {
            printf("error : line %d) the number of the operands is not compatible with the operand (%s)\n"
                    ,fileLineNumber, operator->opName);
            isFoundError = 1;
        }
    }

    if (operator->numOfOperands ==1)
    {
        insLin->dataOPs[0] = checkAndProccessAM(line, operator, DEST);
        addLineData(insLin, lineType_INSTRUCTION);
        IC += 2;
    }
    if(operator->numOfOperands == 0) /* if operand is 14 or 15 (rts , stop)*/
    {
        int  foundAttemptedParameters = 0;
        i = 0;
        while (line[i] != '\0' && ! foundAttemptedParameters)
        {
            if (!isspace(line[i]))
            {
                foundAttemptedParameters = 1;
            }
            i++;
        }
        if(foundAttemptedParameters)
        {
            printf("error : line %d) this instruction shouldn't get parameters (%s)\n"
                    ,fileLineNumber, operator->opName);
            isFoundError = 1;
        }
        else
        {
            addLineData(insLin, lineType_INSTRUCTION);
            IC += 1;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function checks for directive and if it found and it is valid , save the data in a struct.
 */
void checkDirective(char *potentialDirective,char *line, char *label, int originalDataCounter)
{
    dataOrString *dataOrStr;
    char *newLabel, **operands;
    int i,size2dArray;
    int *arrOfInts = NULL , *operand = NULL;

    if (strcmp(potentialDirective, ".data") == 0)
    {
        if (fewOperands(line))/* if there are more than one operand*/
        {
            if (!validNumOfCommas(line))
            {
                printf("error : line %d) illegal number of commas\n",fileLineNumber);
                isFoundError = 1;
                return;
            }            
            operands = splitStringByChar(line, COMMA);/*split into strings*/
            size2dArray = sizeOfSplittedString(operands);
            arrOfInts = (int*)mallocate(size2dArray * sizeof(int));

            for (i = 0; i < size2dArray; i++)/*checks if numbers*/
            {
                if (!isDigits(operands[i]))/* data should get only digits or singed numbers*/
                {
                    printf("error : line %d) illegal value with .data directive (%s)\n",fileLineNumber, operands[i]);
                    isFoundError = 1;
                    free2dArray(operands, size2dArray);
                    free(arrOfInts);
                    return;
                }
              

                arrOfInts[i] = atoi(operands[i]); /* convert the operand to an int*/
                if (arrOfInts[i] < -16384 || arrOfInts[i] > 16383)/*value is not bigger then 15 bits*/
                {
                    printf("error : line %d) number is out of limits", arrOfInts[i]);
                    isFoundError = 1;
                }

            }

            dataOrStr = updateDataOrString(arrOfInts, DATA, size2dArray);
            addLineData(dataOrStr, lineType_DIRECTIVE);
            DC += size2dArray;

            free2dArray(operands, size2dArray);
        }
        else /* if we have only one operand*/
            {
                if (!isDigits(line)) /* data should get only digits or singed numbers*/
                {
                    printf("error : line %d) illegal value with .data directive (%s)\n", fileLineNumber,line);
                    isFoundError = 1;
                    return;
                }
                /*if we're here , operands is valid*/
                operand = mallocate(sizeof(int ));
                operand[0] = atoi(line); /* convert the operand to an int*/

                if(operand[0]  < -16384 || operand[0] > 16383)/*value is not bigger then 15 bits*/
                {
                    printf("error : line %d) number is out of limits",operand[0]);
                    isFoundError = 1;
                }


                DC += 1;
                dataOrStr = updateDataOrString(operand, DATA, 1);
                addLineData(dataOrStr, lineType_DIRECTIVE);
            }

        if (label) /* if we had label at the beginning, update symbols table*/
        {
            updateLabel(label, labelType_DIRECTIVE, originalDataCounter);
        }
    }

    else if (strcmp(potentialDirective, ".string") == 0)
    {
        char *extractedString;
        extractedString = getBetweenQuotationMarks(line); /* get string between " "*/
        if (!extractedString)
        {
            printf("error : line %d) no string was given\n",fileLineNumber);
            isFoundError = 1;
        }
        else
            { /* if found string */
            int sizeOfExcStr = strlen(extractedString) + 1;
            int *stringByAsciiValues;
            stringByAsciiValues = (int*)mallocate(sizeOfExcStr * sizeof(int));

            for (i = 0; i < sizeOfExcStr - 1; i++)
            {
                stringByAsciiValues[i] = extractedString[i];/* copy the ascii chars of the string that has been found */
            }

            stringByAsciiValues[i] = 0; /* add end of string */

            dataOrStr = updateDataOrString(stringByAsciiValues, DATA, sizeOfExcStr);
            addLineData(dataOrStr, lineType_DIRECTIVE);
            DC += sizeOfExcStr;

            if (label) /* if we had label at the beginning, update symbols table */
            {
                updateLabel(label, labelType_DIRECTIVE, originalDataCounter);
            }
        }
    }
    else if (strcmp(potentialDirective, ".entry") == 0)
    {
        /* checks for entry's label */
        newLabel = (char *)stringDup(line); /* should be the label name */
            if(isValidLabel(newLabel))
            {
                updateEntry(newLabel);
            }
            else
            {
                printf("error : line %d) label is not valid (%s)\n" ,fileLineNumber,newLabel);
                isFoundError = 1;
            }
        }
    else if (strcmp(potentialDirective, ".extern") == 0)
    {
        /*checks for extern's label */
        newLabel = (char *)stringDup(line);
        if (!isLabelExist(newLabel))
        {
            if (isValidLabel(newLabel))
                updateLabel(newLabel, labelType_EXTERN, 0);

            else
            {
                printf("error : line %d) label is not valid (%s)\n" ,fileLineNumber,newLabel);
                isFoundError = 1;
            }

        }
        else
        {
            printf("error : line %d) label is already exist (%s)\n", fileLineNumber,label);
            isFoundError = 1;
        }
        free(newLabel);
    }
    else
    {
        /* the read function should skip empty line but for the check if it is not empty line so */
        /* the input is not a directive, instruction or label, its undefined */
        int emptyLine = 1;
        i=0;
        while(!IS_END_OF_STRING(potentialDirective[i]))
        {
            if(!isspace(potentialDirective[i]))
            emptyLine = 0;
            i++;
        }
        if(!emptyLine)
        {
            printf("error : line %d) undefined input (%s)\n",fileLineNumber,potentialDirective);
            isFoundError = 1;
        }

    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function gets source or destination operand, checks it's validity and returns
 the data of it(Addressing Method and value).
 */
dataOperand * checkAndProccessAM(char * operand, aOperator * data , location sd)
{
    adressingMethods *addMethods = sd == SOURCE ? data->source : data->dest;
    dataOperand *dataOp = NULL;

    if (operand[0] == '#') /* if addressing method  == INSTANT */
    {
        if (isHaveAM(addMethods, INSTANT))
        {
            if (!isDigits(operand + 1)) /* if what comes after # is valid digits(can be signed) */
            {
                printf("error : line %d) expected operand after #, (%s) ins't an operand\n"
                        ,fileLineNumber, (operand + 1));
                isFoundError = 1;
            }
            else
            {
                int tempInt = atoi(operand+1);
                if(tempInt > 2047 || tempInt < -2048) /*value is not bigger the 12 bits */
                {
                    printf("error : line %d) number is out of limits",operand[0]);
                    isFoundError = 1;
                }

            }
        }
        else
        {
            printf("error : line %d) incompatible %s addressing method (%d) with operator (%s)\n"
                    ,fileLineNumber,sd == SOURCE ? "source" : "destination" ,INSTANT,data->opName);
            isFoundError = 1;
        }

        dataOp = fillDataOperand(INSTANT, operand);
    }

    else if (operand[0] == '*')/* if addressing method  == REGISTER_INDIRECT */
    {
        if (isHaveAM(addMethods, REG_INDIRECT))
        {
            if (!isReg(operand + 1)) /* if what comes after * is a valid Reg name */
            {
                printf("error : line %d) expected reg name, (%s) ins't a reg name\n"
                        ,fileLineNumber, (operand + 1));
                isFoundError = 1;
            }
        }
        else
        {
            printf("error : line %d) incompatible addressing method (%d) with operator (%s)\n"
                    ,fileLineNumber,REG_INDIRECT,data->opName);
            isFoundError = 1;
        }
        dataOp = fillDataOperand(REG_INDIRECT, operand);
    }

    else if (isReg(operand)) /* if addressing method  == REGISTER_DIRECT */
    {
        if (!isHaveAM(addMethods, REG_DIRECT))
        {
            printf("error : line %d) incompatible addressing method (%d) with operator (%s)\n"
                    ,fileLineNumber,REG_DIRECT,data->opName);
            isFoundError = 1;
        }
        dataOp = fillDataOperand(REG_DIRECT, operand);
    }
    else
    { /* addressing method == DIRECT, which is label */

        if (isHaveAM(addMethods, DIRECT))
        {
            if (!isValidLabel(operand))
            {
                printf("error : line %d) label is not valid (%s)\n",fileLineNumber, operand);
                isFoundError = 1;
            }
        }
        else
        {
            printf("error : line %d) incompatible addressing method (%d) with operator (%s)\n"
                    ,fileLineNumber,DIRECT,data->opName);
            isFoundError = 1;
        }

        dataOp = fillDataOperand(DIRECT, operand);
    }
    return dataOp;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function update .data or .string information in a struct
*/
dataOrString  *updateDataOrString(int *operands, typeOfData typOfData,int size)
{
    dataOrString * dataOrStr;
    dataOrStr = mallocate(sizeof(dataOrString));

    dataOrStr->data = operands;
    dataOrStr->size = size;
    dataOrStr->dcAdress = DC;

    return dataOrStr;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function add a full line information to a struct
example : instruction command and it's operands and addressing methods
        or a directive information (ints for data, or string info)
*/
void addLineData(void * data , lineType linTyp )
{
    lineData * linData;
    linData = mallocate(sizeof(lineData));

    linData->data = data;
    linData->lineTyp = linTyp;
    linData->next = NULL;
    linData->line = fileLineNumber;

    if(!head) /* if first Node in the list */
    {
        head = linData;
        tail = linData ;
    }
    else
    {
        tail->next = linData;
        tail = linData;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
this function fill the a struct with an operand and it's addressing method
 the info that is given here has already been checked before and must be valid!
 */
dataOperand *fillDataOperand(adressingMethods method, char * operand)
{
    dataOperand * dataOp = mallocate(sizeof(dataOperand));

    dataOp->operand = (char *)stringDup(operand);
    dataOp->addMethod = method;
    dataOp->ARE = 4;

    return dataOp;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 * this function gets a line and check if the line is contain more than only one operand.
 */
int fewOperands(char * operands)
{
    int i = 0  ,foundOperand = 0 , foundSpace = 0 ;

    i = 0 ;
    while (!IS_END_OF_STRING(operands[i]))
    {
        if(!isspace(operands[i])) /* if it found anything but space, consider as operand */
            foundOperand =1;

        if(isspace(operands[i]) && foundOperand)  /* if it found a space and already has operand so it is space between
                                                 operands */
            foundSpace = 1;

        if(operands[i] == ',')  /* if found comma consider that he is separate between operands
                                 later we will check if validity of the commas */
            return TRUE;

        if(!isspace(operands[i]) && foundSpace) /* if it found another operand and has "space between operands" before
                                                 there are few operands */
            return TRUE;
        i++;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function is the second pass, after all data is storage in structs
 going through of these and make sure to update the address of every label there was given as operand or as entry
 also for for reg and digits operands, isolate the digits from the rest of the string
 example r7 -> 7
 #-3 ->  -3
 */
void secondPass()
{
    lineData *currentLineData = head;
    insLine* insLin;
    int numOps = 0, i;
    int newLabelAdress;

    if (!head) /* if we had any directive or instruction during the program */
    {
        printf("No Directive or Instruction lines\n");
    }
    else
    {
       while (currentLineData)
       {
           if(currentLineData->lineTyp == lineType_INSTRUCTION) /* check if the line is Instruction (mov,cmp, etc..) */
           {
               insLin = (insLine *)currentLineData->data; /*since lineData is generic , we use cast (insLine*) */
               numOps = insLin->operator->numOfOperands;
               for(i = 0 ; i < numOps ; i ++)
               {
                   if(insLin->dataOPs[i]->addMethod == DIRECT) /* check if addressing method is direct(label name) */
                   {
                       newLabelAdress = getLabelAdress(insLin->dataOPs[i]->operand); /* check the table
                                                                     for the label and get its address */
                       if(newLabelAdress > -1) /* if it found the label's address */
                       {
                           if(getLabelType(insLin->dataOPs[i]->operand) == labelType_EXTERN)
                           { /* using this function only after we make sure the label is exist! */
                               insLin->dataOPs[i]->ARE = 1; /* if extern put E */
                           }
                           else
                           {
                               insLin->dataOPs[i]->ARE = 2; /* if local so put R */
                           }
                          insLin->dataOPs[i]->operandSecondPassVal = newLabelAdress; /* the operand name (was label name)
                                                                                     to be the address */
                       }
                       else
                       {
                           printf("error : line %d) label doesn't exist (%s)\n"
                                   ,currentLineData->line,insLin->dataOPs[i]->operand);
                           isFoundError = 1;
                       }
                   }
                   else /* it is REG or INSTANT method */
                   {
                       int j ;

                           j = 0;

                           while (!isdigit(insLin->dataOPs[i]->operand[j]))
                           {
                               if(IS_SIGNED(insLin->dataOPs[i]->operand[j]))
                               {
                                   break;
                               }
                               j++;

                           }
                           insLin->dataOPs[i]->operandSecondPassVal = atoi(insLin->dataOPs[i]->operand + j);
                   }
               }
           }

           currentLineData = currentLineData->next;
       }
        fixEntryAdress();
        buildExternList();
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function in the nutshell is a reset before we are going through the next assembler file
 it free all lists, and initiate all external - global variables
 */
void freeAllLists()
{
    int i;
    lineData * currentLineData = head ,* toFreeLineData;
    entry * currentEntryHead = entryListHead, * toFreeEntry;
    

    while(currentLineData != NULL)
    {
        toFreeLineData = currentLineData;
        currentLineData = currentLineData->next;

        if(toFreeLineData->lineTyp == lineType_INSTRUCTION)
        {/*free all instructions line structs */
           freeInsLine(toFreeLineData);
        }
        else if (toFreeLineData->lineTyp == lineType_DIRECTIVE)
        {/*free all data or string structs */
           freeDataOrString(toFreeLineData);
        }
    }
 
    while(currentEntryHead != NULL)
    {/* free all entry structs */
        toFreeEntry = currentEntryHead;
        currentEntryHead = currentEntryHead->next;
        freeEntry(toFreeEntry);
    }
    for(i = 0 ; i < labelSize ; i ++)
    {/*free entire symbol table */
        free(labels[i].labelName);
    }
    if(labelSize >0)
    free(labels);
    freeExternList();

    /*initiate all counters. */
    labelSize = 0;
    IC = 100 ;
    DC = 0 ;
    isFoundError = 0;
    fileLineNumber = 0;

    entryListHead = NULL;
    entryListTail = NULL;

    externListHead = NULL;
    externListTail = NULL;

    head = NULL;
    tail = NULL;


}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function gets a instruction node from the FreeAllLists function and free it
 */
void freeInsLine(lineData * toFreeLineData)
{
    int i,numOps ;
    insLine * curInsLine;
    curInsLine = (insLine *) toFreeLineData->data;
    numOps = curInsLine->operator->numOfOperands; 
    

    

    for( i = 0 ; i < numOps ; i ++)
    {
        free(curInsLine->dataOPs[i]->operand);
        free(curInsLine->dataOPs[i]);
    }
    /* curInsLine->operator  was not dynamic mallocated */
    free(toFreeLineData->data);
    free(toFreeLineData);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
  this function gets a directive node from the FreeAllLists function and free it
 */
void freeDataOrString(lineData * toFreeLineData)
{
    dataOrString * cureDataOrString;
    cureDataOrString = (dataOrString *)toFreeLineData->data;
    free(cureDataOrString->data);
    free(toFreeLineData->data);
    free(toFreeLineData);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function gets a entry node from the FreeAllLists function and free it
 */
void freeEntry(entry * toFreeEntry)
{
    free(toFreeEntry->labelName);
    free(toFreeEntry);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function free the extern list
 */
void freeExternList()
{
    externData * curExtData = externListHead, * tempExtData = NULL;

    while(curExtData)
    {
        tempExtData = curExtData;
        curExtData = curExtData->next;
        free(tempExtData->labelName);
        if(tempExtData->size > 0)
        free(tempExtData->adressesItHasBeenUsed);
        free(tempExtData);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
