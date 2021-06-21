#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "instruction.h"
#include "writeFiles.h"
#include "opcodes.h"
#include "memFunctions.h"
#include "labels.h"

int IC;
int DC;

entry * entryListHead;

symbolsTable * labels;
int labelSize;

externData * externListHead;


lineData *head;
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function makes the object file
 */
void writeOctFile(char * programName)
{
    unsigned int lineBinary  ; /* the binary representation for the instruction line*/

    int i, numOps;
    location sd;
    lineData * currentLineData= head;
    dataOrString * curDataOrStr = NULL;
    insLine * curInsLine = NULL;
    char * fileEnding = ".ob"; /* the ending of the oct output file*/
    char * newFileName = mallocate(strlen(programName) + strlen(fileEnding) + 1);
    strcpy(newFileName,programName);

    strcat(newFileName,fileEnding);
    FILE *fp  = fopen(newFileName,"w");
    if(!fp)
    {
        printf("error : cannot open file\n");
        exit(1);
    }

    fprintf(fp,"  %d   %d\n",IC-100 ,DC); /* first line in the ob file*/

    while(currentLineData)
    {

        if(currentLineData->lineTyp == lineType_INSTRUCTION)
        {
            curInsLine = (insLine*) currentLineData->data;
            lineBinary = getInsLineBits(curInsLine); /* get the instruction binary line*/

            fprintf(fp,"%04d %05o\n",curInsLine->icAdress, lineBinary);

            numOps = curInsLine->operator->numOfOperands;

                for(i = 0; i < numOps ; i++)
                {
                    if(areBothAmsRegs(numOps,curInsLine))
                    {/* if both regs should only use one line for bots*/
                        lineBinary = getBothAreRegBits(curInsLine);/* get the binary line of both regs*/
                        fprintf(fp,"%04d %05o\n",curInsLine->icAdress+i+1, lineBinary);
                        break;
                    }
                    else
                    {
                        sd = numOps == 1 ? DEST : (i==0 ? SOURCE : DEST);/* checks of dest or source*/
                        lineBinary = getOpBits(curInsLine->dataOPs[i],sd); /* get binary for operand*/
                        fprintf(fp,"%04d %05o\n",curInsLine->icAdress+i+1, lineBinary);
                    }

                }
        }
        currentLineData = currentLineData->next;
    }

    currentLineData= head;
    while (currentLineData) /* fill directive at the end of the file*/
    {
        if(currentLineData->lineTyp == lineType_DIRECTIVE)
        {
            curDataOrStr  = (dataOrString *)currentLineData->data;
            for(i = 0 ; i < curDataOrStr->size ; i++)
            {
                lineBinary = getCellDataBits(curDataOrStr->data[i]); /* get binary of the data or string chars\numbers*/
                fprintf(fp,"%04d %05o\n",IC+curDataOrStr->dcAdress +i-1, lineBinary);
            }
        }
        currentLineData = currentLineData->next;
    }
    fclose(fp);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function gets the addressing method bits
 */
unsigned int getAdressingMethodBit(adressingMethods curAM)
{
    if(curAM == INSTANT) return 1; /* 0001*/
    else if(curAM == DIRECT) return 2; /*0010*/
    else if(curAM == REG_INDIRECT) return 4; /* 0100*/
    else if(curAM == REG_DIRECT) return 8; /*1000*/
    else return 0; /* 0000*/
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function get the operand bits
 */
unsigned int getOpBits(dataOperand * dataOp,location sd)
{
    int instantMethodBits = 0;
    unsigned int lineBinary = 0; /* the binary representation for the instruction line*/
    unsigned int valueToSet = 0; /* what value to put in the binary line*/
    unsigned int offset = 0;  /* the position of where to set the value in the binary line*/

    valueToSet = dataOp->ARE; /* get the ARE bits*/
    lineBinary |= valueToSet; /* set the ARE bits*/

    if(dataOp->addMethod == INSTANT)
    {

      offset = 3; /* move to the operand sections*/
      instantMethodBits = dataOp->operandSecondPassVal;
      instantMethodBits = instantMethodBits  < 0 ? 4096+instantMethodBits : instantMethodBits;
      valueToSet = instantMethodBits;
      valueToSet <<= offset;
      lineBinary |= valueToSet;
      return lineBinary;
    }
    else if(dataOp->addMethod == REG_DIRECT || dataOp->addMethod == REG_INDIRECT)
    {
        offset = (sd == DEST ? 3: 6);
        valueToSet = dataOp->operandSecondPassVal;
        valueToSet <<= offset;
        lineBinary |= valueToSet;
        return lineBinary;
    }
    else /* DIRECT, label type*/
    {

        offset = 3;
        if(dataOp->ARE == 2) /* if label is local*/
        {

            if(dataOp->operandSecondPassVal < 100) /* Directive label and its address is the DC only*/
            valueToSet = dataOp->operandSecondPassVal+IC; /* get the operand bits*/
            else /* >100, its address is already the IC and no need to add it*/
            valueToSet = dataOp->operandSecondPassVal;
        }

        else/* if label is external (address is 0)*/
            valueToSet = dataOp->operandSecondPassVal; /* get the operand bits*/
        valueToSet <<= offset; /* move the the bits to the operand's bits section*/
        lineBinary |= valueToSet; /* set the operand's bits*/

        return lineBinary;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function gets the instruction line bits
 */
unsigned int getInsLineBits(insLine * curLineData)
{
    unsigned int lineBinary = 0; /* the binary representation for the instruction line*/
    unsigned int valueToSet = 0; /* what value to put in the binary line*/
    unsigned int offset = 0;  /* the position of where to set the value in the binary line*/


    int numOps = curLineData->operator->numOfOperands;


    /* the first binary line of the instruction*/
    valueToSet = 4; /* A bit from ARE*/
    lineBinary |= valueToSet ; /* put A in ARE section (100) , the default*/

    valueToSet = 0;
    if(numOps==2)/* if we have 2 AMs we start with dest and then source*/
    {
        offset += 3;/* move to the destination addressing method section*/
        valueToSet = getAdressingMethodBit(curLineData->dataOPs[1]->addMethod);
        valueToSet <<=offset;
        lineBinary |= valueToSet;


        valueToSet = 0;
        /*   it will be the second*/
        offset += 4;/* move to the destination addressing method section(if numOps == 2 it is source now (3+3=6))*/
        valueToSet = getAdressingMethodBit(curLineData->dataOPs[0]->addMethod);
        valueToSet <<=offset;
        lineBinary |= valueToSet;
    }
    else if(numOps == 1)
    {
        valueToSet = 0;
        /* if we have only 1 op it will be the first, if 2 it will be the second*/
        offset += 3;/* move to the destination addressing method section*/
        valueToSet = getAdressingMethodBit(curLineData->dataOPs[0]->addMethod);
        valueToSet <<=offset;
        lineBinary |= valueToSet;
    }


    offset = 11; /* the position of the opcodes section*/
    valueToSet = curLineData->operator->opcodeNum;
    valueToSet <<= offset;
    lineBinary |= valueToSet;

    return lineBinary;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function gets the cell of .string or .data bits
 */
unsigned int getCellDataBits(int numToConvert)
{
     return numToConvert < 0 ? 32768+numToConvert : numToConvert;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function checks if both addressing method are reg (REG_INDIRECT or REG_DIRECT)
 */
int areBothAmsRegs(int numOps, insLine * curInsLine )
{
    if(numOps < 2)
        return FALSE;

    return ( curInsLine->dataOPs[0]->addMethod == REG_INDIRECT ||
            curInsLine->dataOPs[0]->addMethod == REG_DIRECT )  &&
            (curInsLine->dataOPs[1]->addMethod == REG_INDIRECT ||
            curInsLine->dataOPs[1]->addMethod == REG_DIRECT) ;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function gets both regs addressing methods one line bits
 */
unsigned int getBothAreRegBits(insLine *curInsLine)
{
    unsigned int lineBinary = 0; /* the binary representation for the instruction line*/
    unsigned int valueToSet = 0; /* what value to put in the binary line*/
    unsigned int offset = 3;  /* number move bits to mov*/


    valueToSet = 4 ; /* prepare the A bit for ARE*/
    lineBinary |= valueToSet ;  /* set the ARE bit*/

    /* set the destination reg number*/
    valueToSet = curInsLine->dataOPs[1]->operandSecondPassVal;
    valueToSet <<= offset;
    lineBinary |= valueToSet;

    /* set the source register number*/
    valueToSet = curInsLine->dataOPs[0]->operandSecondPassVal;
    valueToSet <<= offset*2;
    lineBinary |= valueToSet;

    return lineBinary;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this function write the entry file
 */
void writeEntryFile(char * programName)
{
    entry * curEntry;
    FILE* fp;
    if(entryListHead != NULL)
    {
        char * fileEnding = ".ent"; /* the ending of the entry output file*/
        char * newFileName = mallocate(strlen(programName) + strlen(fileEnding) + 1);
        strcpy(newFileName,programName);

        strcat(newFileName,fileEnding);
        fp  = fopen(newFileName,"w");
        if(!fp)
        {
            printf("error : cannot open file\n");
            exit(1);
        }
        curEntry = entryListHead;
        while(curEntry)
        {
            fprintf(fp,"%s %d\n",curEntry->labelName,curEntry->adress);
            curEntry = curEntry->next;
        }
        fclose(fp);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*
 this file write the extern file
 */
void writeExternFile(char * programName)
{

    FILE *fp;
    externData * curExtData = externListHead;
    char *fileEnding = ".ext"; /* the ending of the extern output file*/
    char *newFileName = mallocate(strlen(programName) + strlen(fileEnding) + 1);
    int size;
    int i;
    strcpy(newFileName, programName);
    strcat(newFileName, fileEnding);
    fp = fopen(newFileName, "w");
    if (!fp)
    {
        printf("error : cannot open file\n");
        exit(1);
    }
    while(curExtData)

    {
        size = curExtData->size;
        for(i = 0 ; i < size ; i++)
        {
            fprintf(fp,"%s %d\n",curExtData->labelName,curExtData->adressesItHasBeenUsed[i]);
        }
        curExtData = curExtData->next;
    }
    fclose(fp);
}
/*--------------------------------------------------------------------------------------------------------------------*/
