#include "first_pass.h"


bool firstPass(FILE *file)
{
	bool pass_success = TRUE;
	char curr_line[MAX_LINE+2];
	int ch;
	
	while(fgets(curr_line, sizeof(curr_line), file) != NULL){
		/*if the line is too long*/
		if(strchr(curr_line,'\n') == NULL){
			/*ERROR*/
			pass_success = FALSE;
			printf("Line is too long\n");

			while ((ch = fgetc(file)) != '\n' && ch != EOF)  continue;
		}
		else{
			printf("###%s",curr_line);
			pass_success = processing_line(curr_line);
			
		}
	}
	return pass_success;
}
	
bool processing_line(char curr_line[MAX_LINE+2])
{
	char **words = separate_line(curr_line);
	bool reading_label = FALSE;
	char *label;
	
	if(words[0][strlen(words[0])-1] == ':')
	{
		reading_label = TRUE;
		printf("IS A LABEL:%s\n",words[0]);
		/*label = malloc(strlen(words[0])-1); 
		if(label == NULL){
			fprintf(stderr, "memory cannot be allocated!!\n");
			return FALSE;
		}
		strcpy(label, words[0]);
		printf("IS A LABEL:%s\n",label);*/
	}

	return TRUE;
}































/*while(fgets(curr_line, sizeof(curr_line), file) != NULL)
	{
		
	

	}
	return pass_success;
}*/
