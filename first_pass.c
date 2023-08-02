#include "first_pass.h"



bool firstPass(FILE *file, long *IC, long *DC, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t)
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
				pass_success = processing_line(curr_line, IC, DC, symbol_table, data_table, code_word_t);	
			}
			free(word);
		}
	}
	

	return pass_success;
}
	
bool processing_line(char curr_line[MAX_LINE+2], long *IC, long *DC, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t)
{
	bool reading_label = FALSE, line_success = TRUE;
	int position = 0;
	char *label = get_next_word(curr_line, &position);
	char *word, *label_to_add;
	char *first_operand = NULL, *second_operand = NULL;
	inst_op opcode;
	
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
				addToTable_symbol (symbol_table, label, DC, date_type);
				(*DC)++;
			}	
			/*add the data to data img: */
			if(!processing_data(curr_line, &position, word, data_table, DC)){
				/*ERROR*/
				line_success = FALSE;
			}
		} 
		else if (strcmp(word, ".extern") == 0)
		{
			printf("extern\n");
			label_to_add = get_next_word(curr_line, &position);
			if(valid_label_mcro(label_to_add))
			{
				printf("IS A LABEL2:%s\n",label_to_add);
				addToTable_symbol (symbol_table, label_to_add, DC, external_type);
				(*DC)++;
			}
			else{
				/*ERROR*/
				line_success = FALSE;
			}
			free(label_to_add);
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
		opcode = stringToEnum(word);

		first_operand = get_next_word(curr_line, &position);
		if(first_operand[strlen(first_operand)-1] == ','){
			first_operand[strlen(first_operand)-1] = '\0';
		}

		skip_whitespace(curr_line, &position);
		if(curr_line[position] == ','){
			position++;			
		}

		second_operand = get_next_word(curr_line, &position);
		processing_instruction(opcode, first_operand, second_operand, code_word_t, IC);

		/*free(first_operand);*/
	}	
	
	free(label);
	free(word);
	
	return line_success;
}


bool processing_data(char curr_line[MAX_LINE+2], int *position, char *directive, data_img **data_table, long *DC)
{
	int binaryArray[MAX_BITS] = {0};
	int number, i, ASCII_char;

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
			if(get_next_num(curr_line, position,&number))
			{		
				printf("number:%d\n",number);
				decToBinary(number,binaryArray, MAX_BITS);
				for (i = 0; i < 12; i++)
				{
					printf("%d", binaryArray[i]);
				}
				printf("\n");

				insert_to_data_img(data_table, DC, binaryArray);
				(*DC)++;
			}
			else{
				/*ERROR*/
				printf("is not a number\n");
				return FALSE;
			}

			if(curr_line[*position] == ',')
			{
				(*position)++;
				skip_whitespace(curr_line, position);

				if(curr_line[*position] == ','){
					/*ERROR*/
					printf("No more than one comma is possible\n");
					return FALSE;
				}
	
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
			printf("The string not start with  a double-quote\n");
			return FALSE;
		}

		(*position)++;
		skip_whitespace(curr_line, position);
		printf("pos:%c\n",curr_line[*position]);
		while(curr_line[*position] != '"' || curr_line[*position] != '\n')
		{
			if(curr_line[*position] == '"')
			{
				for (i = 0; i < MAX_BITS; i++) {
					binaryArray[i] = 0;
				}
				insert_to_data_img(data_table, DC, binaryArray);
				printf("first-dc:%ld\n",*DC);
				(*DC)++;
				break;
			}
			else if(isprint(curr_line[*position]))
			{
				ASCII_char = curr_line[*position];		
				decToBinary(ASCII_char,binaryArray,MAX_BITS);
				for (i = 0; i < 12; i++)
				{
					printf("%d", binaryArray[i]);
				}
				printf("\n");

				insert_to_data_img(data_table, DC, binaryArray);
				printf("first-dc:%ld\n",*DC);

				(*DC)++;

				(*position)++;
				skip_whitespace(curr_line, position);
			} 
			else{
				/*ERROR*/
				printf("The string not finish with  a double-quote\n");
				return FALSE;
			}	
		}
	}
	return TRUE;
}


bool insert_to_data_img(data_img **head, long *new_DC, int binaryArray[]) {
	int i;
	data_img *new_node = (data_img *)malloc(sizeof(data_img));
	if (new_node == NULL) {
		printf("Memory allocation failed.\n");
		return FALSE;
	}
	
	/* Initialize the new node with the given values*/
	new_node->DC = *new_DC;
	for (i = 0; i < MAX_BITS; i++) {
		new_node->data[i] = binaryArray[i];
	}

	for (i = 0; i < 12; i++)
	{
		printf("%d", new_node->data[i]);
	}
	printf("\n");
	printf("DC:%ld\n",new_node->DC);

	new_node->next = NULL; 

	if (*head == NULL) {
		printf("NULL\n");
		*head = new_node;
	} else {
		data_img *current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}

	return TRUE;
}


bool processing_instruction(inst_op opcode, char *first_operand, char *second_operand, code_word **code_word_t, long *IC)
{
	type_op first_op_type, second_op_type;
	bool one_operand = FALSE, regSrc = FALSE;
	int word_array[MAX_BITS] = {0};
	int i;

	printf("opcode: %d\n",opcode);

	printf("first_operand: %s\n",first_operand);
	first_op_type = get_type_op(first_operand);
	printf("first_op_type: %d\n",first_op_type);

	if(first_op_type == REGISTER) regSrc = TRUE;

	printf("second_operand: %s\n",second_operand);
	second_op_type = get_type_op(second_operand);
	printf("second_op_type: %d\n",second_op_type);

	
	if(first_op_type == INCORRECT || second_op_type == INCORRECT){
		/*ERROR*/
		printf("one or two operands are INCORRECT\n");
		return FALSE;
	}
	
	switch (opcode) {
		case OP_RTS:
		case OP_STOP:	
			if(first_op_type == NO_OPERAND && second_op_type == NO_OPERAND)
			{
				/**insert to table*/

				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;
			}
			else{
				/*ERROR*/
				printf("the opcode not match the operands\n");
				return FALSE;
			}
			break;

		case OP_NOT:
		case OP_CLR:
		case OP_INC:
		case OP_DEC:
		case OP_JMP:
		case OP_BNE:
		case OP_RED:
		case OP_JSR:
			if(first_op_type != NO_OPERAND && first_op_type != NUMBER && second_op_type == NO_OPERAND)
			{
				one_operand = TRUE;
				regSrc = FALSE;
				/**insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;

				word_by_type(first_op_type, first_operand, word_array, regSrc);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;
			}
			else{
				/*ERROR*/
				printf("the opcode not match the operands\n");
				return FALSE;
			}
			break;

		case OP_PRN:
			if(first_op_type != NO_OPERAND && second_op_type == NO_OPERAND)
			{
				one_operand = TRUE;
				regSrc = FALSE;
				/**insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;

				word_by_type(first_op_type, first_operand, word_array, regSrc);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;
			}
			else{
				/*ERROR*/
				printf("the opcode not match the operands\n");
				return FALSE;
			}
			break;

		case OP_MOV:
		case OP_ADD:
		case OP_SUB:
			if(first_op_type != NO_OPERAND && second_op_type != NO_OPERAND && second_op_type != NUMBER)
			{
				/**insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;
	
				if(first_op_type == REGISTER && second_op_type == REGISTER){
					word_regi((first_operand[2]-'0'), (second_operand[2]-'0'), word_array);
					insert_to_code_word(code_word_t, IC, word_array);
					(*IC)++;
				}
				else{
					word_by_type(first_op_type, first_operand, word_array,regSrc);
					insert_to_code_word(code_word_t, IC, word_array);
					(*IC)++;

					word_by_type(second_op_type, second_operand, word_array,regSrc);
					insert_to_code_word(code_word_t, IC, word_array);
					(*IC)++;
				}
			}
			else{
				/*ERROR*/
				printf("the opcode not match the operands\n");
				return FALSE;
			}
			break;

		case OP_CMP:
			if(first_op_type != NO_OPERAND && second_op_type != NO_OPERAND)
			{
				/**insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;

				if(first_op_type == REGISTER && second_op_type == REGISTER){
					word_regi((first_operand[2]-'0'), (second_operand[2]-'0'), word_array);
					insert_to_code_word(code_word_t, IC, word_array);
					(*IC)++;
				}
				else{
					word_by_type(first_op_type, first_operand, word_array,regSrc);
					insert_to_code_word(code_word_t, IC, word_array);
					(*IC)++;

					word_by_type(second_op_type, second_operand, word_array,regSrc);
					insert_to_code_word(code_word_t, IC, word_array);
					(*IC)++;
				} 
			}
			else{
				/*ERROR*/
				printf("the opcode not match the operands\n");
				return FALSE;
			}
			break;

		case OP_LEA:
			if(first_op_type == LABEL && second_op_type != NO_OPERAND && second_op_type != NUMBER)
			{
				/**insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;

				word_by_type(first_op_type, first_operand, word_array,regSrc);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;

				word_by_type(second_op_type, second_operand, word_array,regSrc);
				insert_to_code_word(code_word_t, IC, word_array);
				(*IC)++;
			}
			else{
				/*ERROR*/
				printf("the opcode not match the operands\n");
				return FALSE;
			}
			break;

		case OP_NONE:
			/*ERROR*/
			printf("Invalid instruction.\n");
			break;
	}		
	return TRUE;
}


bool insert_to_code_word(code_word **head, long *new_IC, int binaryArray[])
{
	int i;
	code_word *new_node = (code_word *)malloc(sizeof(code_word));
	if (new_node == NULL) {
		printf("Memory allocation failed.\n");
		return FALSE;
	}
	
	/* Initialize the new node with the given values*/
	new_node->IC = *new_IC;
	for (i = 0; i < MAX_BITS; i++) {
		new_node->data[i] = binaryArray[i];
	}

	for (i = 0; i < 12; i++)
	{
		printf("%d", new_node->data[i]);
	}
	printf("\n");
	printf("IC:%ld\n",new_node->IC);

	new_node->next = NULL; 

	if (*head == NULL) {
		printf("NULL\n");
		*head = new_node;
	} else {
		code_word *current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}

	return TRUE;
}


void word_by_type(type_op type, char *operand, int word_array[], bool regSrc)
{
	int i;
	if(type == LABEL)
	{
		for (i = 0; i < MAX_BITS; i++)
		{
			word_array[i] = 2;
		}
	}
	else if(type == REGISTER)
	{
		if(regSrc){
			word_regi((operand[2]-'0'), 0, word_array);
		}
		else{
			word_regi(0, (operand[2]-'0'), word_array);
		}
	}
	else{ /* type == NUMBER */
		word_number(atof(operand), word_array);
	}
}

