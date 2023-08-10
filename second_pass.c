#include "second_pass.h"



bool secondPass(char *fileName, FILE *file, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t, long *IC, long *DC)
{
	bool pass_success = TRUE;
	char curr_line[MAX_LINE+2];
	int position = 0;
	char *word; 
	char *ext_file_name = concat_str(fileName, ".ext");
	FILE *ext_file = fopen(ext_file_name, "w");
	int num_of_line = 0;
	
	/* if unable to open the file: */
	if(ext_file == NULL){
		fprintf(stderr, "ERROR - Unable to open %s.ext! \n", fileName);
		free(ext_file_name);
		return FALSE;
	}	


	while(fgets(curr_line, sizeof(curr_line), file) != NULL){
		num_of_line++;
		position = 0;
		word = get_next_word(curr_line, &position);

		/*If the line is a comment or empty:*/
		if(word == NULL || word[0] == '\0' || word[0] == ';'){
			continue;
		}

		if(!processing_line2(curr_line, symbol_table, num_of_line)){
			/*ERROR*/
			fprintf(stderr,"ERROR in line %d - The content of the line is incorrect\n", num_of_line);
			pass_success = FALSE;
		}
		free(word);
	}


	if(!write_ent_file(fileName, symbol_table)){
		/*ERROR*/
		fprintf(stderr,"ERROR - Something went wrong with the Entry file\n");
		pass_success = FALSE;
	}

	if(!decode_labeles(ext_file, symbol_table, code_word_t, num_of_line)){
		/*ERROR*/
		fprintf(stderr,"ERROR - Something went wrong with the decode\n");
		pass_success = FALSE;
	}


	if(!write_ob_file(fileName, data_table, code_word_t, IC, DC)){
		/*ERROR*/
		fprintf(stderr,"ERROR - Something went wrong with the Object file\n");
		pass_success = FALSE;
	}

	fclose(ext_file);
	free(ext_file_name);

	return pass_success;
}


bool processing_line2(char curr_line[MAX_LINE+2], symbol_t *symbol_table, int num_of_line)
{
	bool line_success = TRUE;
	int position = 0, symbol_location;
	char *symbol, *word = get_next_word(curr_line, &position);

	/*skip the first word if it is a label:*/
	if(word[strlen(word)-1] == ':')
	{
		word = get_next_word(curr_line, &position);
	}
	
	if (strcmp(word, ".entry") == 0)
	{
		skip_whitespace(curr_line, &position);
		/* We will check if there is a comma before the first label:*/
		if(curr_line[position] == ',')
		{
			/*ERROR*/
			fprintf(stderr, "ERROR in line %d - There must not be a comma before the first label\n", num_of_line);
			line_success = FALSE;
		}

		symbol = get_next_word(curr_line, &position);
		symbol_location = search_symbol(symbol_table, symbol);
		/*check if the label does exist in the symbol table:*/
		if(symbol_location == -1){
			/*ERROR*/
			fprintf(stderr,"ERROR in line %d - Symbol not found\n", num_of_line);
			line_success = FALSE;	
		}
		else{
			if(symbol_table->values[symbol_location].type == EXTERNAL_TYPE){
				/*ERROR*/
				fprintf(stderr,"ERROR in line %d - Symbol is already defined as external\n", num_of_line);
				line_success = FALSE;
			}
			symbol_table->values[symbol_location].type = ENTRY_TYPE;
		}

		/* We will go over the whole line and add the labels to symbol table: */
		while(curr_line[position] != '\0')
		{
			if(curr_line[position] == ',')
			{
				position++;
				skip_whitespace(curr_line, &position);
			
				/* We will check that there is no more than one comma: */
				if(curr_line[position] == ','){
					/*ERROR*/
					fprintf(stderr, "ERROR in line %d - It is not possible to have two or more consecutive commas\n", num_of_line);
					return FALSE;
				}
			}
			else{
				/*ERROR*/
				fprintf(stderr, "ERROR in line %d - Between two labels must be one comma\n", num_of_line);
				return FALSE;
			}

			symbol = get_next_word(curr_line, &position);
			symbol_location = search_symbol(symbol_table, symbol);
			/*check if the label does exist in the symbol table:*/
			if(symbol_location == -1){
				/*ERROR*/
				fprintf(stderr,"ERROR in line %d - Symbol not found\n", num_of_line);
				line_success = FALSE;	
			}
			else{
				symbol_table->values[symbol_location].type = ENTRY_TYPE;
			}

		}

		/* We will check that a comma does not appear after the last label: */ 
		if(curr_line[position] == ','){				
			/*ERROR*/
			fprintf(stderr, "ERROR in line %d - It is not possible to have comma/s after the last label\n", num_of_line);
			return FALSE;
		}
		free(symbol);			
	}

	free(word);
	return line_success;
}


bool write_ent_file(char *fileName, symbol_t *symbol_table)
{
	char *ent_file_name = concat_str(fileName, ".ent");
	FILE *ent_file = fopen(ent_file_name, "w");
	int i;
	
	/* if unable to open the file: */
	if(ent_file == NULL){
		fprintf(stderr, "ERROR - Unable to open %s.ent! \n", fileName);
		free(ent_file_name);
		return FALSE;
	}	

	/*We will go through all the symbols in the table and extract to the file those of the entry type:*/
	for (i=0; i<symbol_table->num_symbols; i++)
	{
		if(symbol_table->values[i].type == ENTRY_TYPE){
			fprintf(ent_file, "%s ", symbol_table->values[i].symbol_name);			
			fprintf(ent_file, "%ld\n", symbol_table->values[i].address);			
		}
	}

	fclose(ent_file);
	free(ent_file_name);

	return TRUE;
}


void write_ext_file(FILE *file, char *symbol_name, long address)
{
	fprintf(file, "%s ", symbol_name);			
	fprintf(file, "%ld\n", address);	
}


bool decode_labeles(FILE *file, symbol_t *symbol_table, code_word **code_word_t, int num_of_line)
{
	code_word* current = *code_word_t;
	int symbol_location;

	while (current != NULL) {
		/*check if the label has not been coded yet:*/
		if(strcmp(current->label, "") != 0)
		{
			symbol_location = search_symbol(symbol_table, current->label);
			if(symbol_location == -1){
				/*ERROR*/
				fprintf(stderr,"ERROR in line %d - Symbol not found\n", num_of_line);
				return FALSE;
			}

			switch(symbol_table->values[symbol_location].type)
			{
				case CODE_TYPE:
				case DATA_TYPE:			
				case ENTRY_TYPE:
					word_number(symbol_table->values[symbol_location].address, current->data);
					current->data[10] = 1;
					break;
				case EXTERNAL_TYPE:
					current->data[11] = 1;
					write_ext_file(file, current->label, current->IC);
					break;
			}
		}
		current = current->next;
	}

	return TRUE;
}


char binaryToBase64Char(int binaryArray[], int start, int end)
{
	int i, value = 0;
	char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	for (i = start; i < end; i++) 
	{
        	value = (value << 1) + binaryArray[i];
    	}

	return base64_chars[value];
}


bool write_ob_file(char *fileName, data_img **data_table, code_word **code_word_t, long *IC, long *DC)
{
	char *ob_file_name = concat_str(fileName, ".ob");
	FILE *ob_file = fopen(ob_file_name, "w");
	code_word* current_c = *code_word_t;
	data_img* current_d = *data_table;
	char first_char, second_char;

	/* if unable to open the file: */
	if(ob_file == NULL){
		fprintf(stderr, "ERROR - Unable to open %s.ob! \n", fileName);
		free(ob_file_name);
		return FALSE;
	}
	
	/*check if*t the memory does not exceed 1024 in the memory*/
	if(*IC + *DC + START_MEMORY > MAX_MEMORY){
		fprintf(stderr, "ERROR - 1024 is the maximum value of the memory! \n");
		fclose(ob_file);
		free(ob_file_name);
		return FALSE;
	}

	fprintf(ob_file, "%ld ", *IC);
	fprintf(ob_file, "%ld\n", *DC);

	/*We will go through the instruction table, encode each word to base 64 and print to an OB file*/
	while (current_c != NULL) 
	{
		first_char = binaryToBase64Char(current_c->data, 0, MAX_BITS/2);	
		second_char = binaryToBase64Char(current_c->data, MAX_BITS/2, MAX_BITS);
		
		fprintf(ob_file, "%c", first_char);		
		fprintf(ob_file, "%c\n", second_char);		

		current_c = current_c->next;
	}

	/*We will go through the data table, encode each word to base 64 and print to an OB file*/
	while (current_d != NULL) 
	{
		first_char = binaryToBase64Char(current_d->data, 0, MAX_BITS/2);	
		second_char = binaryToBase64Char(current_d->data, MAX_BITS/2, MAX_BITS);
		
		fprintf(ob_file, "%c", first_char);		
		fprintf(ob_file, "%c\n", second_char);		

		current_d = current_d->next;
	}

	fclose(ob_file);
	free(ob_file_name);

	return TRUE;
}

