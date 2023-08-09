#include "second_pass.h"



bool secondPass(char *fileName, FILE *file, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t)
{
	bool pass_success = TRUE;
	char curr_line[MAX_LINE+2];
	int position = 0;
	char *word; 
	char *ext_file_name = concat_str(fileName, ".ext");
	FILE *ext_file = fopen(ext_file_name, "w");
	
	/* if unable to open the file: */
	if(ext_file == NULL){
		fprintf(stderr, "ERROR - Unable to open %s.ext! \n", fileName);
        	fclose(ext_file);
		free(ext_file_name);
		return FALSE;
	}	


	while(fgets(curr_line, sizeof(curr_line), file) != NULL){
		printf("--------------------------\n");
		position = 0;
		word = get_next_word(curr_line, &position);
		/*If the line is a comment or empty:*/
		if(word == NULL || word[0] == '\0' || word[0] == ';'){
			printf("A comment or blank line\n");
			continue;
		}
		printf("###%s",curr_line);
		if(!processing_line2(curr_line, symbol_table)){
			/*ERROR*/
			fprintf(stderr,"ERROR - The content of the line is incorrect\n");
			pass_success = FALSE;
		}
		free(word);
	}


	if(!write_ent_file(fileName, symbol_table)){
		/*ERROR*/
		fprintf(stderr,"ERROR - Something went wrong with the Entry file\n");
		pass_success = FALSE;
	}

	if(!decode_labeles(ext_file, symbol_table, code_word_t)){
		/*ERROR*/
		fprintf(stderr,"ERROR - Something went wrong with the decode\n");
		pass_success = FALSE;
	}


	if(!write_ob_file(fileName, data_table, code_word_t)){
		/*ERROR*/
		fprintf(stderr,"ERROR - Something went wrong with the Object file\n");
		pass_success = FALSE;
	}

	fclose(ext_file);
	free(ext_file_name);

	return pass_success;
}


bool processing_line2(char curr_line[MAX_LINE+2], symbol_t *symbol_table)
{
	bool line_success = TRUE;
	int position = 0, symbol_location;
	char *symbol = get_next_word(curr_line, &position);
	char *word = get_next_word(curr_line, &position);

	if(word[strlen(word)-1] == ':')
	{
		word = get_next_word(curr_line, &position);
	}
	
	if (strcmp(word, ".entry") == 0)
	{
		symbol = get_next_word(curr_line, &position);
		symbol_location = search_symbol(symbol_table, symbol);
		if(symbol_location == -1){
			/*ERROR*/
			fprintf(stderr,"ERROR - Symbol not found\n");
			printf("symbol not found");
			line_success = FALSE;	
		}
		else{
			symbol_table->values[symbol_location].type = ENTRY_TYPE;
		}
		

	}

	free(symbol);
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
        	fclose(ent_file);
		free(ent_file_name);
		return FALSE;
	}	


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


bool decode_labeles(FILE *file, symbol_t *symbol_table, code_word **code_word_t)
{
	code_word* current = *code_word_t;
	int symbol_location;

	while (current != NULL) {
		if(strcmp(current->label, "") != 0)
		{
			symbol_location = search_symbol(symbol_table, current->label);
			if(symbol_location == -1){
				/*ERROR*/
				fprintf(stderr,"ERROR - Symbol not found\n");
				printf("symbol not found");
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


bool write_ob_file(char *fileName, data_img **data_table, code_word **code_word_t)
{
	char *ob_file_name = concat_str(fileName, ".ob");
	FILE *ob_file = fopen(ob_file_name, "w");
	code_word* current_c = *code_word_t;
	data_img* current_d = *data_table;
	char first_char, second_char;

	/* if unable to open the file: */
	if(ob_file == NULL){
		fprintf(stderr, "Unable to open %s.ob! \n", fileName);
        	fclose(ob_file);
		free(ob_file_name);
		return FALSE;
	}
	
	while (current_c != NULL) 
	{
		first_char = binaryToBase64Char(current_c->data, 0, MAX_BITS/2);	
		second_char = binaryToBase64Char(current_c->data, MAX_BITS/2, MAX_BITS);
		
		fprintf(ob_file, "%c", first_char);		
		fprintf(ob_file, "%c\n", second_char);		

		current_c = current_c->next;
	}

	while (current_d != NULL) 
	{
		first_char = binaryToBase64Char(current_d->data, 0, 6);	
		second_char = binaryToBase64Char(current_d->data, 6, MAX_BITS);
		
		fprintf(ob_file, "%c", first_char);		
		fprintf(ob_file, "%c\n", second_char);		

		current_d = current_d->next;
	}

	fclose(ob_file);
	free(ob_file_name);

	return TRUE;
}

