#ifndef ASSEMBLER_OPCODES_H
#define ASSEMBLER_OPCODES_H

#define NUMBER_OF_OPERATORS 16

typedef enum  {INSTANT, DIRECT, REG_INDIRECT, REG_DIRECT,INVALID}adressingMethods; /* what addressing method */
typedef enum {DATA,STRING}typeOfData; /* what type of data */


/* the struct of the operators information */
typedef struct /* this struct is the information about the  instruction commands */
{
    char * opName;
    int numOfSourceAdressing;
    int numOfDestAdressing;
    adressingMethods source[4];
    adressingMethods dest[4];
    int numOfOperands;
    int opcodeNum;

}aOperator;

/* this struct represent an operand and its addressing method */
typedef struct
{
    adressingMethods addMethod;
    char * operand;
    int operandSecondPassVal;
    int ARE;

}dataOperand;

/* instruction command and the operands it received */
typedef struct
{
    aOperator * operator;
    dataOperand *dataOPs[2];
    int icAdress; /* the instruction counter value */
}insLine;

/* this struct represent data or string and its operands or chars */
typedef struct
{
    typeOfData typOfData; /* .data or .string */
    int * data; /* array of ascii values  \ numbers */
    int size; /* size of the array */
    int line; /* what line it was found in the code */
    int dcAdress; /* the data counter value */
}dataOrString;


extern aOperator aOperators[NUMBER_OF_OPERATORS];
#endif /* ASSEMBLER_OPCODES_H */
