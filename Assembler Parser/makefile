all: assembler
assembler: main.o instruction.o labels.o memFunctions.o opcodes.o readAssemblerFile.o stringFunctions.o writeFiles.o
	gcc -ansi -Wall -pedantic -o assembler main.o instruction.o labels.o memFunctions.o opcodes.o readAssemblerFile.o stringFunctions.o writeFiles.o
main.o: main.c instruction.h readAssemblerFile.h writeFiles.h
	gcc -ansi -Wall -pedantic -c main.c
instruction.o: instruction.c instruction.h opcodes.h stringFunctions.h memFunctions.h labels.h
	gcc -ansi -Wall -pedantic -c instruction.c
labels.o: labels.c labels.h instruction.h memFunctions.h opcodes.h stringFunctions.h
	gcc -ansi -Wall -pedantic -c labels.c
memFunctions: memFunctions.c 
	gcc -ansi - Wall -pedantic -c memFunctions.c
opcodes: opcodes.c opcodes.h
	gcc -ansi -Wall -pedantic -c opcodes.c
readAssemblerFile: readAssemblerFile.c readAssemblerFile.h instruction.h
	gcc -ansi -Wall -pedantic -c readAssemblerFIle.c
stringFunctions: stringFunctions.c stringFunctions.h memFunctions.h instruction.h
	gcc -ansi -Wall - pedantic -c stringFunctions.c
writeFiles: writeFiles.c instruction.h writeFiles.h opcodes.h memFunctions.h labels.h
	gcc -ansi -Wall -pedantic -c writeFiles.c
