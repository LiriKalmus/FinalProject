#include "first_pass.h"



bool firstPass(FILE *file, long *IC, long *DC, symbol_t *symbol_table, data_img *data_table)
{
	bool pass_success = TRUE;
	char curr_line[MAX_LINE+2];
	int ch, position = 0;
	char *word; 

	while(fgets(curr_line, sizeof(curr_line), file) != NULL){
		printf("--------------------------\n");
		/*if the line is too long*/
		if(strchr(curr_line,'\n') == NULL){
			/*ERROR*/
			pass_success = FALSE;
			printf("Line is too long\n");

			while ((ch = fgetc(file)) != '\n' && ch != EOF)  continue;
		}
		else{
			position = 0;
			word = get_next_word(curr_line, &position);
			/*If the line is a comment or empty:*/
			if(word == NULL || word[0] == '\0' || word[0] == ';'){
				printf("A comment or blank line\n");
				continue;
			}
			else{
				printf("###%s",curr_line);
				pass_success = processing_line(curr_line, IC, DC, symbol_table, data_table);	
			}
			free(word);
		}
		/*printSymbolTable(symbol_table);*/
	}

	return pass_success;
}
	
bool processing_line(char curr_line[MAX_LINE+2], long *IC, long *DC, symbol_t *symbol_table, data_img *data_table)
{
	bool reading_label = FALSE, line_success = TRUE;
	int position = 0;
	char *label = get_next_word(curr_line, &position);
	char *word, *first_operand, *second_operand;
	inst_op opcode;
	type_op type_opcode;
/*	
	printf("first:%d\n",strlen(word));
	printf("curr_line:%s\n",curr_line);
*/
	
	
	if(label[strlen(label)-1] == ':')
	{
		label[strlen(label)-1] = '\0';
		if(valid_label_mcro(label)){
			printf("IS A LABEL:%s\n",label);
			reading_label = TRUE;
		}
		else{
			/*ERROR*/
			line_success = FALSE;
		}
	}
	else{
		position = 0;
	}
	

	word = get_next_word(curr_line, &position);
	if(word[0] == '.')
	{		
		printf("directive:%s\n",word);
		
		if (strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0)
		{
			if(reading_label)
			{
				addToTable_symbol (symbol_table, label, &DC, date_type);
			}	
			/*add to data img*/
			add_data_to_data_img(curr_line, &position, word, data_table);

		} 
		else if (strcmp(word, ".extern") == 0)
		{
			printf("extern\n");
			/*addToTable_symbol (symbol_table, curr_line, position, 0, external_type);*/
		} 
		/*else if (strcmp(word, ".entry") == 0) 
		{
			printf("entry\n");
			continue;
		} */
		else if (strcmp(word, ".entry") != 0) 
		{
			printf("Invalid choice\n");
			/*ERROR*/
			line_success = FALSE;
		}
	}
	else /* the word should be opcode*/
	{
		first_operand = get_next_word(curr_line, &position);
		type_opcode = get_type_op(first_operand));
		opcode = stringToEnum(word);

		if(opcode == OP_RTS || opcode == OP_STOP){
			if(type_opcode != NO_OPERAND){
				/*ERROR*/
				printf("No operands are needed.\n");
				line_success = FALSE;
			}
			/* ic++ */
		}
		/*switch (opcode) {
			case OP_RTS:
			case OP_STOP:
				if(first_operand != NULL || first_operand[0] != '\0'){
					/*ERROR*/
		/*			printf("No operands are needed.\n");
					line_success = FALSE;
				}
				break;
			/*if(first_operand == NULL || first_operand[0] == '\0'){
				/*ERROR*/
				/*printf("missing operand.\n");
				line_success = FALSE;
			}*/
		/*	case OP_NOT:
			case OP_CLR:
			case OP_INC:
			case OP_DEC:
			case OP_JMP:
			case OP_BNE:
			case OP_RED:
			case OP_PRN:
			case OP_JSR:
				printf("need 1 parameter\n");
				break;

			case OP_MOV:
			case OP_CMP:
			case OP_ADD:
			case OP_SUB:
			case OP_LEA:
				printf("need 2 parameters\n");
				break;

			case OP_NONE:
				/*ERROR*/
		/*		printf("Invalid instruction.\n");
				line_success = FALSE;
				break;
	    	}*/
		free(first_operand);
	}	
	
	free(label);
	free(word);
	
	return line_success;
}





bool add_data_to_data_img(char curr_line[MAX_LINE+2], int *position, char *directive, data_img *data_table)
{
	int number;

	if(strcmp(directive, ".data") == 0)
	{	
		skip_whitespace(curr_line, position);
		if(curr_line[*position] == ',')
		{
			/*ERROR*/
			printf("Comma before the first number\n");
			return FALSE;
		}
		while(curr_line[*position] != '\0')
		{
			if(get_next_num(curr_line, position,&number)){
				/*number correct - add to data table*/		
				printf("number:%d\n",number);
			}
			else{
				/*ERROR*/
				printf("is not a number\n");
				return FALSE;
			}
			if(curr_line[*position] == ','){
				(*position)++;
				skip_whitespace(curr_line, position);
				if(curr_line[*position] == '\0'){				
					/*ERROR*/
					printf("Comma after the last number\n");
					return FALSE;
				}
			}
		}
	}
	else{ /* .string */
		skip_whitespace(curr_line, position);
		if(curr_line[*position] != '"')
		{
			/*ERROR*/
			printf("The string does not start with  a double-quote\n");
			return FALSE;
		}
		(*position)++;
		skip_whitespace(curr_line, position);
		while(curr_line[*position] != '"')
		{
			if(isprint(curr_line[*position])){
				/*character correct - add to data table*/		
				printf("character:%c\n",curr_line[*position]);
				(*position)++;
				skip_whitespace(curr_line, position);
			}
			else{
				/*ERROR*/
				printf("is not a character\n");
				return FALSE;
			}
		}
	}
	return TRUE;
}













