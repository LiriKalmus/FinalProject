#include "pre_assembly.h" 
#include "tables.h"

bool pre_assembly_file(char *fileName)
{
	char *as_fileName = concat_fileName(fileName, ".as");
	FILE *as_file = fopen(as_fileName, "r");
	char *am_full_name;
	FILE *am_file;
	char curr_line[MAX_LINE], line[MAX_LINE];
	char *word;
	bool reading_mcro = FALSE;
	mcro_t* mcro_t = createTable_mcro();

	/* if unable to open the file: */
	if(as_file == NULL){
		fprintf(stderr, "Unable to open %s.as! \n", fileName);
		free(as_fileName);
        	free(as_file);
		return FALSE;
	}
	
	am_full_name = concat_fileName(fileName, ".am");
	am_file = fopen(am_full_name, "w");
	/* if unable to open the file: */
	if(am_file == NULL){
		fprintf(stderr, "Unable to open %s.am! \n", fileName);
		free(as_fileName);
        	free(as_file);
		free(am_full_name);
        	free(am_file);
		return FALSE;
	}	
	
	while(fgets(curr_line, MAX_LINE+2, as_file) != NULL) {
		strcpy(line, curr_line);
		word = strtok(line, " ");
		
		/*if(_find(word)){
			fputs(_get(word), am_file);
		}
		else*/ 
		
		if(strcmp(word, "mcro")==0){
			word = strtok(NULL, " ");
			printf("%s\n",word);
			/*_add(word);*/
			reading_mcro = TRUE;
		}
		else if(strncmp(word, "endmcro",7)==0){
			printf("h\n");
			reading_mcro = FALSE;
		}
		
		else if(reading_mcro == TRUE){
			
			/*_add(curr_line);*/
		}
		else{
			fputs(curr_line, am_file);
		}
		
	}
	return TRUE;
}
