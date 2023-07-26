#include "pre_assembly.h"

bool pre_assembly(char *fileName)
{
	char *as_fileName = concat_str(fileName, ".as");
	FILE *as_file = fopen(as_fileName, "r");
	char *am_full_name;
	FILE *am_file;
	
	char *curr_line = NULL;
	size_t max_len = 0;
	int ch, i = 0;
	char **words;

	char *mcro_name, *mcro_definition = "";
	bool reading_mcro = FALSE;
	mcro_t *mcro_table = createTable_mcro();

	/* if unable to open the file: */
	if(as_file == NULL){
		fprintf(stderr, "Unable to open %s.as! \n", fileName);
		free(as_fileName);
        	fclose(as_file);
		freeMcro_t(mcro_table);
		return FALSE;
	}
	
	am_full_name = concat_str(fileName, ".am");
	am_file = fopen(am_full_name, "w");
	/* if unable to open the file: */
	if(am_file == NULL){
		fprintf(stderr, "Unable to open %s.am! \n", fileName);
		free(as_fileName);
        	fclose(as_file);
		free(am_full_name);
        	fclose(am_file);
		freeMcro_t(mcro_table);
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
		
			/*Separating the line into an array of words:*/
			words = separate_line(curr_line);
							
			/* If the first field is a macro name listed in the macro table*/
			if(getFromTable_mcro(mcro_table, words[0]) != NULL)
			{
				fputs(getFromTable_mcro(mcro_table, words[0]), am_file);
			}
			/* If this is the start of a macro definition*/
			else if(strncmp(words[0], "mcro",4)==0)
			{
				/*If the line length of the macro definition is longer than 80*/
				if(strlen(curr_line) > MAX_LINE+2){
					fprintf(stderr, "the mcro name is too long\n");
					free(as_fileName);
					fclose(as_file);
					free(am_full_name);
					fclose(am_file);
					freeMcro_t(mcro_table);
					free(curr_line);
					i = 0;
					if (words != NULL) {
				    		while (words[i] != NULL) { free(words[i++]); }
						free(words);  
					}
					return FALSE;
				}
								
				mcro_name= malloc(strlen(words[1])+1);
				if(mcro_name == NULL){
					fprintf(stderr, "memory cannot be allocated!!\n");
					free(as_fileName);
					fclose(as_file);
					free(am_full_name);
					fclose(am_file);
					freeMcro_t(mcro_table);
					free(curr_line);
					i = 0;
					if (words != NULL) {
				    		while (words[i] != NULL) { free(words[i++]); }
						free(words);
					}  
					return FALSE;
				}
				strcpy(mcro_name, words[1]);
				reading_mcro = TRUE;
			}

			/*If this is the end of defining a macro*/
			else if(strncmp(words[0], "endmcro",7)==0)
			{
				addToTable_mcro(mcro_table,mcro_name,mcro_definition);
				strcpy(mcro_definition,"");
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
		}
	}
		
	free(as_fileName);
        fclose(as_file);
	free(am_full_name);
        fclose(am_file);
	
	free(mcro_definition);
        free(mcro_name);
	freeMcro_t(mcro_table);
	
	i = 0;
	if (words != NULL) {
    		while (words[i] != NULL) { free(words[i++]); }
		free(words);
	}  
	free(curr_line);  

	return TRUE;
}
