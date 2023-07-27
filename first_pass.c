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
	bool reading_label = FALSE, line_success = TRUE;
	int word_position = 0;
	
	if(words[word_position][strlen(words[word_position])-1] == ':')
	{
		if(valid_label(words[word_position])){
			reading_label = TRUE;
		}
		else{
			/*ERROR*/
			line_success = FALSE;
		}
		word_position++;	
	}

	if(words[word_position][0] == '.'){
		if(valid_directive(words[word_position])){
			printf("directive:%s\n",words[word_position]);
			
			if (strcmp(words[word_position], "data") == 0 || strcmp(words[word_position], "string") == 0)
			{
				if(reading_label){

				}
			} 
			else if (strcmp(words[word_position], "entry") == 0) 
			{
				printf("entry\n");
			} 
			else if (strcmp(words[word_position], "extern") == 0) 
			{
				printf("extern\n");
			} 
			else {
				printf("Invalid choice\n");
				line_success = FALSE;
			}
		}
		else{
			/*ERROR*/
			line_success = FALSE;
		}
		word_position++;	
	}

	return line_success;
}































/*while(fgets(curr_line, sizeof(curr_line), file) != NULL)
	{
		
	

	}
	return pass_success;
}*/
