#include "pre_assembly.h"
#include "tables.h" 

bool pre_assembly_file(char *fileName)
{
	char *as_fileName = concat_str(fileName, ".as");
	FILE *as_file = fopen(as_fileName, "r");
	char *am_full_name;
	FILE *am_file;
	char curr_line[MAX_LINE], line[MAX_LINE];
	char *word, *mcro_name, *mcro_definition = "";
	bool reading_mcro = FALSE;
	mcro_t *mcro_table = createTable_mcro();

	/* if unable to open the file: */
	if(as_file == NULL){
		fprintf(stderr, "Unable to open %s.as! \n", fileName);
		free(as_fileName);
        	free(as_file);
		freeMcro_t(mcro_table);
		return FALSE;
	}
	
	am_full_name = concat_str(fileName, ".am");
	am_file = fopen(am_full_name, "w");
	/* if unable to open the file: */
	if(am_file == NULL){
		fprintf(stderr, "Unable to open %s.am! \n", fileName);
		free(as_fileName);
        	free(as_file);
		free(am_full_name);
        	free(am_file);
		freeMcro_t(mcro_table);
		return FALSE;
	}	
	
	
	while(fgets(curr_line, MAX_LINE+2, as_file) != NULL) {
		strcpy(line, curr_line);
		word = strtok(line, " ");
		printf("/////%s\n",word);
		if(getFromTable_mcro(mcro_table, word) != NULL){
			printf("/////%s\n",word);
			fputs(getFromTable_mcro(mcro_table, word), am_file);
		}
		else if(strncmp(word, "mcro",4)==0){
			word = strtok(NULL, " ");
			printf(")()(%s\n",word);
			mcro_name= malloc(strlen(word)+1);
			strcpy(mcro_name, word);
			printf("!@!@!@%s\n",mcro_name);
			reading_mcro = TRUE;
		}
		else if(strncmp(word, "endmcro",7)==0){
			printf("!!!%s\n",word);
			printf("!@#!@#%s\n",mcro_definition);
			printf("$#$#%s\n",mcro_name);
			addToTable_mcro(mcro_table,mcro_name,mcro_definition);
			reading_mcro = FALSE;
		}
		
		else if(reading_mcro == TRUE){
			mcro_definition = concat_str(mcro_definition, curr_line);
			printf("$$$$$$%s\n",mcro_definition);
			printf("@@@@!%s\n",mcro_name);
		}
		else{
			fputs(curr_line, am_file);
		}
		
	}

	free(as_fileName);
        free(as_file);
	free(am_full_name);
        free(am_file);
        free(mcro_definition);
        free(mcro_name);
	freeMcro_t(mcro_table);
	return TRUE;
}
