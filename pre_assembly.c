#include "pre_assembly.h"

bool pre_assembly(char *fileName)
{
	char *as_fileName = concat_str(fileName, ".as");
	FILE *as_file = fopen(as_fileName, "r");
	char *am_full_name;
	FILE *am_file;
	
	char *curr_line = NULL;
	size_t max_len = 0;
	int ch, i = 0, position = 0;
	char *word = NULL;

	bool reading_mcro = FALSE;
	mcro_t *mcro_table = createTable_mcro();
	char *mcro_name = NULL, *mcro_definition = (char *)malloc(1);

	int num_of_line = 0;
	
	mcro_definition[0] = '\0';
	
	/* if unable to open the file: */
	if(as_file == NULL){
		fprintf(stderr, "ERROR - Unable to open %s.as! \n", fileName);
		free(as_fileName);
		freeMcro_t(mcro_table);
		free(mcro_definition);
		return FALSE;
	}
	
	am_full_name = concat_str(fileName, ".am");
	am_file = fopen(am_full_name, "w");
	/* if unable to open the file: */
	if(am_file == NULL){
		fprintf(stderr, "ERROR - Unable to open %s.am! \n", fileName);
		free(as_fileName);
        	fclose(as_file);
		free(am_full_name);
		freeMcro_t(mcro_table);
		free(mcro_definition);
		return FALSE;
	}	
	
	/*Read lines from the file: */
	while ((ch = fgetc(as_file)) != EOF) {
		if (i == max_len) {
			max_len = (max_len == 0) ? 1 : max_len * 2; 
			curr_line = realloc(curr_line, max_len);
		}
		curr_line[i++] = ch;
		
		/* If we have finished reading the line:*/
		if (ch == '\n') {
			curr_line[i] = '\0'; 
			i = 0;
			position = 0;
			num_of_line++;
			word = get_next_word(curr_line, &position);

							
			/* check if the macro name is not a label: */
			if(word[strlen(word)-1] == ':')
			{
				word[strlen(word)-1] = '\0';
				if(getFromTable_mcro(mcro_table, word) != NULL){
					fprintf(stderr, " ERROR in line %d - the mcro name can not be a label\n", num_of_line);
					fclose(as_file);
					free(as_fileName);
					free(am_full_name);
					fclose(am_file);
					freeMcro_t(mcro_table);
					free(curr_line);
					free(word);
					free(mcro_definition);
					free(mcro_name);
					return FALSE;
				}
			}

			/* If the first field is a macro name listed in the macro table*/
			if(getFromTable_mcro(mcro_table, word) != NULL)
			{
				/*If the line length is longer than 80*/
				if(i > MAX_LINE+2){
					fprintf(stderr, " ERROR in line %d - the line is too long\n", num_of_line);
					fclose(as_file);
					free(as_fileName);
					free(am_full_name);
					fclose(am_file);
					freeMcro_t(mcro_table);
					free(curr_line);
					free(word);
					free(mcro_definition);
					free(mcro_name);
					return FALSE;
				}
				if(!suffix_line(curr_line, &position, num_of_line)){
					fclose(as_file);
					free(as_fileName);
					free(am_full_name);
					fclose(am_file);
					freeMcro_t(mcro_table);
					free(curr_line);
					free(word);
					free(mcro_definition);
					free(mcro_name);
					return FALSE;
				}
												
				fputs(getFromTable_mcro(mcro_table, word), am_file);
			}
			/* If this is the start of a macro definition*/
			else if(strncmp(word, "mcro",4)==0)
			{
				/*If the line length is longer than 80*/
				if(i > MAX_LINE+2){
					fprintf(stderr, " ERROR in line %d - the line is too long\n", num_of_line);
					fclose(as_file);
					free(as_fileName);
					fclose(am_file);
					free(am_full_name);
					freeMcro_t(mcro_table);
					free(curr_line);
					free(word);
					free(mcro_definition);
					free(mcro_name);
					return FALSE;
				}

				mcro_name = get_next_word(curr_line, &position);
				if(!valid_label_mcro(mcro_name)){
					fprintf(stderr, "ERROR in line %d - the mcro name is invalid\n", num_of_line);
					fclose(as_file);
					free(as_fileName);
					fclose(am_file);
					free(am_full_name);
					freeMcro_t(mcro_table);
					free(curr_line);
					free(word);
					free(mcro_definition);
					free(mcro_name);
					return FALSE;					
				}

				if(getFromTable_mcro(mcro_table, mcro_name) != NULL){
					fprintf(stderr, "ERROR in line %d - the mcro name is already defined\n", num_of_line);
					fclose(as_file);
					free(as_fileName);
					fclose(am_file);
					free(am_full_name);
					freeMcro_t(mcro_table);
					free(curr_line);
					free(word);
					free(mcro_definition);
					free(mcro_name);
					return FALSE;	
				}


				if(!suffix_line(curr_line, &position, num_of_line)){
					fclose(as_file);
					free(as_fileName);
					fclose(am_file);
					free(am_full_name);
					freeMcro_t(mcro_table);
					free(curr_line);
					free(word);
					free(mcro_definition);
					free(mcro_name);					
					return FALSE;
				}
				reading_mcro = TRUE;	
			}

			/*If this is the end of defining a macro*/
			else if(strncmp(word, "endmcro",7)==0)
			{
				/*If the line length is longer than 80*/
				if(i > MAX_LINE+2){
					fprintf(stderr, " ERROR - the mcro name is too long\n");
					fclose(as_file);
					free(as_fileName);
					fclose(am_file);
					free(am_full_name);
					freeMcro_t(mcro_table);
					free(curr_line);
					free(word);
					free(mcro_definition);
					return FALSE;
				}

				if(!suffix_line(curr_line, &position, num_of_line)){
					fclose(as_file);					
					free(as_fileName);
					fclose(am_file);
					free(am_full_name);
					freeMcro_t(mcro_table);
					free(curr_line);
					free(word);
					free(mcro_definition);
					free(mcro_name);
					return FALSE;
				}
				
				addToTable_mcro(mcro_table,mcro_name,mcro_definition);
				free(mcro_definition); 
        			mcro_definition = (char *)malloc(1);
				mcro_definition[0] = '\0';
				reading_mcro = FALSE;
			}

			/* If this line is part of a macro definition */
			else if(reading_mcro == TRUE)
			{
				mcro_definition = concat_str(mcro_definition, curr_line);
			}

			/* Otherwise - write the line to the .am file */
			else{
				fputs(curr_line, am_file);
			}					
		free(word);
		}	
	}

	fclose(as_file);	
	free(as_fileName);

        fclose(am_file);
	free(am_full_name);


	freeMcro_t(mcro_table);
	free(mcro_name);
	free(mcro_definition);	
	free(curr_line);  

	return TRUE;
}
