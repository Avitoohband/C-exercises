#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"

#include "readAssemblerFile.h"
#include "writeFiles.h"

int IC;
int DC;

int isFoundError;

int main(int argc, char *argv[])
{
int i = 1;
    if(argc <2) /* if there are not arguments*/
    {
        printf("At least one argument expected\n");
        exit(0);
    }
        
        while(i < argc  )
        {
            readAssembler(argv[i]); /* first pass on the file*/

            if(IC+DC >4096) /* check there is no memory exceed*/
            {
                printf("Program exceed from memory limits(bigger then 4096)\n");
                isFoundError = 1;

            }
            if(isFoundError)
            {
                printf("There was errors during the program, output files will not be made\n");
            }

            else
            {
                secondPass(); /* second pass on the file*/
                if(!isFoundError) /* second pass can find errors like labels that were not declared, etc...*/
                {
                    writeOctFile(argv[i]); /* make object file (octal file)  . ob*/
                    writeEntryFile(argv[i]); /* entry file   .ent*/
                    writeExternFile(argv[i]); /* extern file   .ext*/

                }
            }
            /*next potential file index*/
            i++;
            /*initiate everything before we run the next potential file.*/
            freeAllLists();

    }
   return 0;
}
