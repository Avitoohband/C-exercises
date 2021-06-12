#ifndef ASSEMBLER_INSTRUCTION_H
#define ASSEMBLER_INSTRUCTION_H

#include "opcodes.h"
#include "labels.h"

typedef enum {SOURCE,DEST} location;  /* to declare if operand is source or destination */
enum BOOLVAL{FALSE,TRUE} ; /* to use as is like boolean */

typedef enum {lineType_DIRECTIVE,lineType_INSTRUCTION} lineType; /* what type or line */

/*generic struct which will contain both of directive or instruction data */
typedef struct lineData
{
    void* data;  /* the directive or instruction struct data */
    lineType lineTyp; /* type of data */
    struct lineData* next;
    int line; /* what line in the file it has been found */
} lineData;


aOperator * getStruct(char * );
void addLineData(void *  , lineType);
void analyseLine (char * );
int isDigits(char * );
dataOperand * checkAndProccessAM(char * , aOperator *  , location );
int isHaveAM(adressingMethods * , adressingMethods );
int isReg(char * );
void checkDirective(char * ,char * , char * , int);
void checkInstruction(char * ,aOperator *);
dataOrString  *updateDataOrString(int *, typeOfData ,int );
dataOperand *fillDataOperand(adressingMethods , char * );
int fewOperands(char * );
void secondPass();
void freeAllLists();
void freeInsLine(lineData * );
void freeDataOrString(lineData * );
void freeExternList();
void freeEntry(entry * );




extern int IC; /* Instruction Counter */
extern int DC; /* Data Counter */
extern lineData *head; /* head of the Line Data list */
extern lineData *tail; /* tail of the Line Data list */

extern int isFoundError; /* error alert */

#endif /*ASSEMBLER_INSTRUCTION_H */
