#include <stdio.h>
#include "debbug.h"
#include "labels.h"
//this file is only for self debbuging and is not part of the project.
int labelSize;
symbolsTable * labels;

lineData *head;

entry *entryListHead;

void printLabels()
{
    int i;
    for(i = 0 ; i < labelSize ; i ++)
    {
        printf("label name :%s\n", labels[i].labelName);
        printf("label address :%d\n", labels[i].adress);
        printf("label type : %d\n",labels[i].typ);
    }
}

void printGenericStruct()
{
    lineData * current = head;
    while(current)
    {
        if(current->data != NULL)
        printGenericNode(current);
        current = current->next;
    }
}

void printGenericNode (lineData * node)
{
    int i;
    if(node->lineTyp == lineType_DIRECTIVE)
    {
        dataOrString * dataOrStr = (dataOrString *) node->data;
        for(i = 0 ; i < dataOrStr->size ; i++)
        {
            printf("%d ",(int)dataOrStr->data[i]);
        }
        putchar('\n');
    }
    if(node->lineTyp == lineType_INSTRUCTION)
    {
        insLine * insLin = (insLine *) node->data;

        printf("%s \n", insLin->operator->opName);
        for(i = 0 ; i < insLin->operator->numOfOperands ; i ++)
        {
            printf("%d \n",insLin->dataOPs[i]->addMethod);
            printf("%s\n",insLin->dataOPs[i]->operand);
        }
    }
}
void printEntry()
{
    entry * current = entryListHead;
    while(current)
    {
        printf("%s\n",current->labelName);
        current = current->next;
    }
}

