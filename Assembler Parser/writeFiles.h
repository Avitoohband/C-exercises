#ifndef ASSEMBLER_WRITEFILES_H
#define ASSEMBLER_WRITEFILES_H

void writeOctFile(char *);
unsigned int getAdressingMethodBit(adressingMethods );
unsigned int convertDecToOctal(unsigned int );
unsigned int getOpBits(dataOperand * ,location);
unsigned int getCellDataBits(int  );
unsigned int getInsLineBits(insLine * );
int areBothAmsRegs(int , insLine *  );
unsigned int getBothAreRegBits(insLine *);
void writeEntryFile(char * );
void writeExternFile(char *);

#endif /*ASSEMBLER_WRITEFILES_H*/
