#include "first_pass.h"



bool firstPass(FILE *file)
{
	bool pass_success = TRUE;
	char curr_line[MAX_LINE+2];
	int ch;
	long IC = 0;
	long DC = 0;
	/*kidudNetonim_t *kidudNetonim_table = createKidudNetonim();*/
	symbol_t *symbol_table = createTable_symbol();

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
			pass_success = processing_line(curr_line, symbol_table/*,kidudNetonim_table, DC*/);	
		}
	printSymbolTable(symbol_table);
	}
	return pass_success;
}
	
bool processing_line(char curr_line[MAX_LINE+2], symbol_t *symbol_table/*,kidudNetonim_t *kidudNetonim_table, long DC*/)
{
	char **words = separate_line(curr_line);
	bool reading_label = FALSE, line_success = TRUE;
	int word_position = 0;
	int char_count=0;
	

	printf("@^^^^^^^@!%s\n",words[word_position]);
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
	}printf("@^#@#@#@#@^@!%s\n",words[word_position]);

	

	if(words[word_position][0] == '.'){
		if(valid_directive(words[word_position])){
			printf("directive:%s\n",words[word_position]);
			
			if (strcmp(words[word_position], "data") == 0 || strcmp(words[word_position], "string") == 0)
			{
				if(reading_label)
				{
				printf("@!!!!!!!!@!%s\n",words[word_position]);
				printf("@!@!%s\n",words[word_position-1]);
				addToTable_symbol (symbol_table, words[word_position-1], 0, date_type);
					
				
					/*while(words[word_position+1][char_count] !='\n' || words[word_position+1][char_count] !=EOF)
					{
						if(words[word_position+1][char_count] =='"')
						char_count++;
						else
						addToKidudNetonim_t(kidudNetonim_table, DC, words[word_position+1][char_count]);
						{
						char_count++;
						DC++;
						}

					}
					char_count=0;*/

				}
			} 
			else if (strcmp(words[word_position], "entry") == 0) 
			{
				printf("entry\n");

			} 
			else if (strcmp(words[word_position], "extern") == 0)
			{
				printf("extern\n");
				addToTable_symbol (symbol_table, words[word_position+1], 8, external_type);
			} 
			else {
				printf("Invalid choice\n");
				line_success = FALSE;
			}
		}
	else if(1==1) 
		{
		printf("$$$$$$$$$$$$$$$$#@$%s\n",words[word_position]);
		}
	/*else if(strcmp(words[word_position], "mov")==0 || strcmp(words[word_position], "cmp")==0 || strcmp(words[word_position], "add")==0 || strcmp(words[word_position], "sub")==0 || strcmp(words[word_position], "not")==0 || strcmp(words[word_position], "clr")==0 || strcmp(words[word_position], "lea")==0 || strcmp(words[word_position], "inc")==0 || strcmp(words[word_position], "dec")==0 || strcmp(words[word_position], "jmp")==0 || strcmp(words[word_position], "bne")==0 || strcmp(words[word_position], "red")==0 || strcmp(words[word_position], "prn")==0 || strcmp(words[word_position], "jsr")==0 || strcmp(words[word_position], "rts")==0 || strcmp(words[word_position], "stop")==0)
	
		addToTable_symbol (symbol_table, words[word_position-1], 16, code_type);
		
		{
			ERROR
			line_success = FALSE;
		}*/
		word_position++;	
	}

	return line_success;
}































/*while(fgets(curr_line, sizeof(curr_line), file) != NULL)
	{
		
	

	}
	return pass_success;
}*/
