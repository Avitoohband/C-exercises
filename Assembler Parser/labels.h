#ifndef ASSEMBLER_LABELS_H
#define ASSEMBLER_LABELS_H
typedef enum  {labelType_EXTERN,labelType_DIRECTIVE,labelType_INSTRUCTION}labelType; /* what label type */

/*this will be the label data or symbols data */
typedef struct
{
    char * labelName;
    int adress;
    labelType typ;

}symbolsTable;

/* entry data */
typedef struct entry
{
    char * labelName;
    int adress;
    struct entry * next;
    int line; /* what line in the code */
}entry;

/* extern data */
typedef struct externData
{
    char * labelName;
    int * adressesItHasBeenUsed; /* where was used in the code (address) */
    int size; /* size of the array of ints (addresses) */
    struct externData * next;
}externData;


extern entry * entryListHead; /* head of the entry list */
extern entry * entryListTail; /* tail of the entry list */

extern externData * externListHead; /* head of the extern list */
extern externData * externListTail; /* tail of the extern list */


extern int labelSize; /* size of the label list */
extern symbolsTable * labels; /* the label list */

void updateLabel(char * , int ,int );
int isValidLabel(char *);
int isLabelExist(char *);
char* checkForLabel(char *line);
void updateEntry(char * );
int getLabelAdress(char *);
int getLabelType(char * );
void fixEntryAdress();
void buildExternList();
void addExternDataToList(externData *);
int isExternCreated(char *);
externData * createNewExtern(char *);
externData * getExternData(char *);
void fillExternUsedAdresses(externData *);

#endif /*ASSEMBLER_LABELS_H */