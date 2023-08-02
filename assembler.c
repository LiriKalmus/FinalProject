#include "assembler.h"


bool file_processing(char *fileName)
{
	char *am_fileName;
	FILE *am_file;
	long IC = 0, DC = 0;
	data_img **data_table = NULL;
	code_word **code_word_t = NULL;
	symbol_t *symbol_table = createTable_symbol();

 	if(!pre_assembly(fileName)){
		fprintf(stderr, "The macros did not spread properly in %s.as! \n", fileName);
		return FALSE;
	}

	am_fileName = concat_str(fileName, ".am");
	am_file = fopen(am_fileName, "r");
	/* if unable to open the file: */
	if(am_file == NULL){
		fprintf(stderr, "Unable to open %s.am! \n", fileName);
		free(am_fileName);
        	fclose(am_file);
		return FALSE;
	}
	
	if(!firstPass(am_file, &IC, &DC, symbol_table, &data_table, &code_word_t)){
		printf("FIRST PASS FAILED");
	}
	printSymbolTable(symbol_table);
	printAllNodes(&data_table);

	freeTable_symbol(symbol_table);
	freeAllNodes(&data_table);

	printAllwords(&code_word_t);

	free(am_fileName);
       	fclose(am_file);
	return TRUE;
}



