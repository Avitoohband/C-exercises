#ifndef ASSEMBLER_READASSEMBLERFILE_H
#define ASSEMBLER_READASSEMBLERFILE_H

void readAssembler(char *);
int checkCommentLine(char * );
int checkEmptyLine(char *);


extern int fileLineNumber; /* line indicator */
#endif /* ASSEMBLER_READASSEMBLERFILE_H */
