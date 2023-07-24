assembler: assembler.o pre_assembly.o utils.o tables.o variables.h 
	gcc -g -ansi -Wall -pedantic assembler.o pre_assembly.o utils.o tables.o -o assembler

assembler.o: assembler.c variables.h 
	gcc -c -ansi -Wall -pedantic assembler.c -g -o assembler.o

pre_assembly.o: pre_assembly.c pre_assembly.h variables.h
	gcc -c -ansi -Wall -pedantic pre_assembly.c -g -o pre_assembly.o

utils.o: utils.c utils.h 
	gcc -c -ansi -Wall -pedantic utils.c -g -o utils.o

tables.o: tables.c tables.h 
	gcc -c -ansi -Wall -pedantic tables.c -g -o tables.o
