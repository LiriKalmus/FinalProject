assembler: main.o assembler.o pre_assembly.o first_pass.o utils.o tables.o variables.h 
	gcc -g -ansi -Wall -pedantic main.o assembler.o pre_assembly.o first_pass.o utils.o tables.o -o assembler

main.o: main.c 
	gcc -c -ansi -Wall -pedantic main.c -g -o main.o

assembler.o: assembler.h assembler.c variables.h 
	gcc -c -ansi -Wall -pedantic assembler.c -g -o assembler.o

pre_assembly.o: pre_assembly.h pre_assembly.c variables.h
	gcc -c -ansi -Wall -pedantic pre_assembly.c -g -o pre_assembly.o

first_pass.o: first_pass.h first_pass.c variables.h
	gcc -c -ansi -Wall -pedantic first_pass.c -g -o first_pass.o

utils.o: utils.h utils.c 
	gcc -c -ansi -Wall -pedantic utils.c -g -o utils.o

tables.o: tables.h tables.c 
	gcc -c -ansi -Wall -pedantic tables.c -g -o tables.o
