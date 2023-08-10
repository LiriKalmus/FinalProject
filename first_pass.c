#include "first_pass.h"



bool firstPass(FILE *file, long *IC, long *DC, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t)
{
	bool pass_success = TRUE;
	char curr_line[MAX_LINE+2];
	int ch, position = 0;
	char *word; 
	int num_of_line = 0;

	while(fgets(curr_line, sizeof(curr_line), file) != NULL)
	{
		num_of_line++;
		/*check if the line is too long:*/
		if(strchr(curr_line,'\n') == NULL){
			/*ERROR*/
			pass_success = FALSE;
			fprintf(stderr, "ERROR in line %d - The line is too long\n", num_of_line);
			while ((ch = fgetc(file)) != '\n' && ch != EOF)  continue;
		}
		else{
			position = 0;
			word = get_next_word(curr_line, &position);
			/*If the line is a comment or empty:*/
			if(word == NULL || word[0] == '\0' || word[0] == ';'){
				fprintf(stdout,"A comment or blank line\n");
				continue;
			}

			if(!processing_line(curr_line, IC, DC, symbol_table, data_table, code_word_t, num_of_line)){
				/*ERROR*/
				pass_success = FALSE;
			}
			free(word);
		}
	}

	/* We will update the memory addresses: */
	update_adresses(code_word_t, data_table, symbol_table);

	return pass_success;
}
	


bool processing_line(char curr_line[MAX_LINE+2], long *IC, long *DC, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t, int num_of_line)
{
	bool reading_label = FALSE, line_success = TRUE;
	int position = 0;
	char *word, *label = get_next_word(curr_line, &position);
	
	/* check if there is a label at the beginning of the line: */
	if(label[strlen(label)-1] == ':')
	{
		label[strlen(label)-1] = '\0';
		if(valid_label_mcro(label)){
			reading_label = TRUE;
		}
		else{
			/*ERROR*/
			fprintf(stderr, "ERROR in line %d - %s: In not a valid label \n",num_of_line, label);
			line_success = FALSE;
		}
	}
	else{
		position = 0;
	}
	
	/* We will pick up the next word and check if it is a directive or an instruction:*/
	word = get_next_word(curr_line, &position);

	if(word[0] == '.') /* If this is a directive */
	{		
		if (strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0)
		{
			if(reading_label && !addToTable_symbol (symbol_table, label, DC, DATA_TYPE)){
				/*ERROR*/
				fprintf(stderr,"ERROR in line %d - Failed to add to symbol table\n", num_of_line);
				line_success = FALSE;
			}

			/*add the data to data img: */
			if(strcmp(word, ".data") == 0){
				if(!processing_data(curr_line, &position, word, data_table, DC, num_of_line)){
					/*ERROR*/
					return FALSE;
				}
			}
			else{ /*add the string to data img: */
				if(!processing_string(curr_line, &position, word, data_table, DC, num_of_line)){
					/*ERROR*/
					return FALSE;
				}
			}

			/*If there are extra characters*/
			if(!suffix_line(curr_line, &position, num_of_line)){
				/*ERROR*/
				line_success = FALSE;
			}
		} 

		else if (strcmp(word, ".extern") == 0)
		{
			if(!add_labels_to_symbol_table(curr_line, &position, num_of_line, symbol_table)){
				/*ERROR*/
				line_success = FALSE;
			}
		} 

		else if(strcmp(word, ".entry") != 0)
		{
			/*ERROR*/
			fprintf(stderr,"ERROR in line %d - Invalid choice. The only choices are: entry, extern, srting, data \n", num_of_line);
			line_success = FALSE;
		}
	}

	else /* the word should be instruction:*/
	{
		if(reading_label && !addToTable_symbol (symbol_table, label, IC, CODE_TYPE)){
			/*ERROR*/
			fprintf(stderr,"ERROR in line %d - Failed to add the label into the symbol table\n", num_of_line);
			line_success = FALSE;
		}

		if(!processing_instruction(curr_line, &position, word, code_word_t, IC, num_of_line)){
			/*ERROR*/
			return FALSE;
		}

		if(!suffix_line(curr_line, &position, num_of_line)){
			/*ERROR*/
			line_success = FALSE;
		}
	}


	free(label);
	free(word);
	
	return line_success;
}


bool processing_data(char curr_line[MAX_LINE+2], int *position, char *directive, data_img **data_table, long *DC, int num_of_line)
{
	int binaryArray[MAX_BITS] = {0};
	int number;

	skip_whitespace(curr_line, position);
	
	/* We will check if there is a comma before the first number:*/
	if(curr_line[*position] == ',')
	{
		/*ERROR*/
		fprintf(stderr, "ERROR in line %d - There must not be a comma before the first number\n", num_of_line);
		return FALSE;
	}

	/* We will take the first number, convert it to binary and put it in the data table: */
	if(get_next_num(curr_line, position,&number))
	{		
		decToBinary(number,binaryArray, MAX_BITS);
		insert_to_data_img(data_table, DC, binaryArray, num_of_line);
		(*DC)++;
	}
	else{
		/*ERROR*/
		fprintf(stderr, "ERROR in line %d: Not a number, this variable must be an integer\n", num_of_line);
		return FALSE;
	}

	/* We will go over the whole line and add the numbers to data table: */	
	while(curr_line[*position] != '\0')
	{
		if(curr_line[*position] == ',')
		{
			(*position)++;
			skip_whitespace(curr_line, position);

			/* We will check that there is no more than one comma: */
			if(curr_line[*position] == ','){
				/*ERROR*/
				fprintf(stderr, "ERROR in line %d - It is not possible to have two or more consecutive commas\n", num_of_line);
				return FALSE;
			}
		}
		else{
			/*ERROR*/
			fprintf(stderr, "ERROR in line %d - Between two numbers must be one comma\n", num_of_line);
			return FALSE;
		}

		/* We will take the next number, convert it to binary and put it in the data table: */
		if(get_next_num(curr_line, position,&number))
		{		
			decToBinary(number,binaryArray, MAX_BITS);
			insert_to_data_img(data_table, DC, binaryArray, num_of_line);
			(*DC)++;
		}
		else{
			/*ERROR*/
			fprintf(stderr, "ERROR in line %d - %d: Not a number, this variable must be an integer\n", num_of_line, number);
			return FALSE;
		}
	}

	/* We will check that a comma does not appear after the last number: */ 
	if(curr_line[*position] == ','){				
		/*ERROR*/
		fprintf(stderr, "ERROR in line %d - It is not possible to have comma/s after the last number\n", num_of_line);
		return FALSE;
	}

	return TRUE;
}

bool processing_string(char curr_line[MAX_LINE+2], int *position, char *directive, data_img **data_table, long *DC, int num_of_line)
{
	int binaryArray[MAX_BITS] = {0};	
	int i, ASCII_char; 

	skip_whitespace(curr_line, position);

	/* We will check if the string starts with a double-quote:*/
	if(curr_line[*position] != '"')
	{
		/*ERROR*/
		fprintf(stderr, "ERROR in line %d - The string must start with a double-quote \n", num_of_line);
		return FALSE;
	}

	(*position)++; /*skip the double-quote*/
	skip_whitespace(curr_line, position);

	/* We will go through all the characters in the string: */
	while(curr_line[*position] != '"' || curr_line[*position] != '\n')
	{
		/*If we received at the end of the string we will add the character '\0' to data table: */
		if(curr_line[*position] == '"')
		{
			(*position)++;
			for (i = 0; i < MAX_BITS; i++) {
				binaryArray[i] = 0;
			}
			insert_to_data_img(data_table, DC, binaryArray, num_of_line);
			(*DC)++;
			break;
		}
		/*encode each character to its ASCII value and then convert to binary and insert into the data table:*/
		else if(isprint(curr_line[*position]))
		{
			ASCII_char = curr_line[*position];		
			decToBinary(ASCII_char,binaryArray,MAX_BITS);
			insert_to_data_img(data_table, DC, binaryArray, num_of_line);
			(*DC)++;
			(*position)++;

		} 
		else{
			/*ERROR*/
			fprintf(stderr, "ERROR in line %d - The string is illegal \n", num_of_line);
			return FALSE;
		}	
	}
	return TRUE;
}

bool insert_to_data_img(data_img **head, long *new_DC, int binaryArray[], int num_of_line) 
{
	int i;
	data_img *new_node = (data_img *)malloc(sizeof(data_img));
	if (new_node == NULL) {
		fprintf(stderr, "ERROR in line %d - Memory allocation failed\n", num_of_line);
		return FALSE;
	}
	
	/* Initialize the new node with the given values*/
	new_node->DC = *new_DC;
	for (i = 0; i < MAX_BITS; i++) {
		new_node->data[i] = binaryArray[i];
	}

	new_node->next = NULL; 

	if (*head == NULL) 
	{
		*head = new_node;
	} 
	else {
		data_img *current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}

	return TRUE;
}


bool add_labels_to_symbol_table(char curr_line[MAX_LINE+2], int *position, int num_of_line, symbol_t *symbol_table)
{
	char *label_to_add;

	skip_whitespace(curr_line, position);

	/* We will check if there is a comma before the first label:*/
	if(curr_line[*position] == ','){
		/*ERROR*/
		fprintf(stderr, "ERROR in line %d - There must not be a comma before the first label\n", num_of_line);
		return FALSE;
	}

	label_to_add = get_next_word(curr_line, position);
	/* We will check if the label is correct: */
	if(valid_label_mcro(label_to_add))
	{
		if(!addToTable_symbol (symbol_table, label_to_add, NULL, EXTERNAL_TYPE)){
			/*ERROR*/
			fprintf(stderr,"ERROR in line %d - Failed to add to symbol table\n", num_of_line);
			return FALSE;
		}
	}
	else{
		/*ERROR*/
		fprintf(stderr,"ERROR in line %d - The label in invalid\n", num_of_line);
		return FALSE;
	}

	/* We will go over the whole line and add the labels to symbol table: */
	while(curr_line[*position] != '\0')
	{
		if(curr_line[*position] == ',')
		{
			position++;
			skip_whitespace(curr_line, position);

			/* We will check that there is no more than one comma: */
			if(curr_line[*position] == ','){
				/*ERROR*/
				fprintf(stderr, "ERROR in line %d - It is not possible to have two or more consecutive commas\n", num_of_line);
				return FALSE;
			}
		}
		else{
			/*ERROR*/
			fprintf(stderr, "ERROR in line %d - Between two labels must be one comma\n", num_of_line);
			return FALSE;
		}

		label_to_add = get_next_word(curr_line, position);
		/* We will check if the label is correct: */
		if(valid_label_mcro(label_to_add))
		{
			if(!addToTable_symbol (symbol_table, label_to_add, NULL, EXTERNAL_TYPE)){
				/*ERROR*/
				fprintf(stderr,"ERROR in line %d - Failed to add to symbol table\n", num_of_line);
				return FALSE;
			}
		}
		else{
			/*ERROR*/
			fprintf(stderr,"ERROR in line %d - The label in invalid\n", num_of_line);
			return FALSE;
		}
	}

	/* We will check that a comma does not appear after the last label: */ 
	if(curr_line[*position] == ','){				
		/*ERROR*/
		fprintf(stderr, "ERROR in line %d - It is not possible to have comma/s after the last label\n", num_of_line);
		return FALSE;
	}

	free(label_to_add);
	return TRUE;
}


bool processing_instruction(char curr_line[MAX_LINE+2], int *position, char *word, code_word **code_word_t, long *IC, int num_of_line)
{
	bool one_operand = FALSE, regSrc = FALSE, isLabel = FALSE;
	int word_array[MAX_BITS] = {0};	
	char *first_operand = NULL, *second_operand = NULL;
	type_op first_op_type, second_op_type;
	inst_op opcode = stringToEnum(word);

	first_operand = get_next_word(curr_line, position);
	first_op_type = get_type_op(first_operand);
	if(first_op_type == REGISTER) regSrc = TRUE;

	skip_whitespace(curr_line, position);
	if(curr_line[*position] == ','){
		(*position)++;			
	}
	
	skip_whitespace(curr_line, position);
	/* We will check that there is no more than one comma: */
	if(curr_line[*position] == ','){
		/*ERROR*/
		fprintf(stderr,"ERROR in line %d - It is not possible to have two or more consecutive commas\n", num_of_line);
		return FALSE;			
	}

	second_operand = get_next_word(curr_line, position);
	second_op_type = get_type_op(second_operand);
	
	/*check if there are no wrong operands:*/ 
	if(first_op_type == INCORRECT || second_op_type == INCORRECT){
		/*ERROR*/
		fprintf(stderr, "ERROR in line %d - One or two operands are INCORRECT\n", num_of_line);
		return FALSE;
	}
	
	switch (opcode) {
		case OP_RTS:
		case OP_STOP:	
			if(first_op_type == NO_OPERAND && second_op_type == NO_OPERAND)
			{
				/*insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, NULL, num_of_line);
				(*IC)++;
			}
			else{   /*the opcode not match the operands*/
				/*ERROR*/
				fprintf(stderr, "ERROR in line %d - The opcode did not match the operands\n", num_of_line);
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
				/*insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, NULL, num_of_line);
				(*IC)++;

				isLabel = word_by_type(first_op_type, first_operand, word_array, regSrc);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, first_operand, num_of_line);
				(*IC)++;
			}
			else{   /*the opcode not match the operands*/				
				/*ERROR*/
				fprintf(stderr, "ERROR in line %d - The opcode did not match the operands\n", num_of_line);
				return FALSE;
			}
			break;

		case OP_PRN:
			if(first_op_type != NO_OPERAND && second_op_type == NO_OPERAND)
			{
				one_operand = TRUE;
				regSrc = FALSE;
				/*insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, NULL, num_of_line);
				(*IC)++;

				isLabel = word_by_type(first_op_type, first_operand, word_array, regSrc);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, first_operand, num_of_line);
				(*IC)++;
			}
			else{   /*the opcode not match the operands*/				
				/*ERROR*/
				fprintf(stderr, "ERROR in line %d - The opcode did not match the operands\n", num_of_line);
				return FALSE;
			}
			break;

		case OP_MOV:
		case OP_ADD:
		case OP_SUB:
			if(first_op_type != NO_OPERAND && second_op_type != NO_OPERAND && second_op_type != NUMBER)
			{
				/*insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, NULL, num_of_line);
				(*IC)++;
	
				if(first_op_type == REGISTER && second_op_type == REGISTER){
					word_regi((first_operand[2]-'0'), (second_operand[2]-'0'), word_array);
					insert_to_code_word(code_word_t, IC, word_array, isLabel, NULL, num_of_line);
					(*IC)++;
				}
				else{
					isLabel = word_by_type(first_op_type, first_operand, word_array,regSrc);
					insert_to_code_word(code_word_t, IC, word_array, isLabel, first_operand, num_of_line);
					(*IC)++;

					isLabel = word_by_type(second_op_type, second_operand, word_array,regSrc);
					insert_to_code_word(code_word_t, IC, word_array, isLabel, second_operand, num_of_line);
					(*IC)++;
				}
			}
			else{   /*the opcode not match the operands*/				
				/*ERROR*/
				fprintf(stderr, "ERROR in line %d - The opcode did not match the operands\n", num_of_line);
				return FALSE;
			}
			break;

		case OP_CMP:
			if(first_op_type != NO_OPERAND && second_op_type != NO_OPERAND)
			{
				/*insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, NULL, num_of_line);
				(*IC)++;

				if(first_op_type == REGISTER && second_op_type == REGISTER){
					word_regi((first_operand[2]-'0'), (second_operand[2]-'0'), word_array);
					insert_to_code_word(code_word_t, IC, word_array, isLabel, NULL, num_of_line);
					(*IC)++;
				}
				else{
					isLabel = word_by_type(first_op_type, first_operand, word_array,regSrc);
					insert_to_code_word(code_word_t, IC, word_array, isLabel, first_operand, num_of_line);
					(*IC)++;

					isLabel = word_by_type(second_op_type, second_operand, word_array,regSrc);
					insert_to_code_word(code_word_t, IC, word_array, isLabel, second_operand, num_of_line);
					(*IC)++;
				} 
			}
			else{   /*the opcode not match the operands*/				
				/*ERROR*/
				fprintf(stderr, "ERROR in line %d - The opcode did not match the operands\n", num_of_line);
				return FALSE;
			}
			break;

		case OP_LEA:
			if(first_op_type == LABEL && second_op_type != NO_OPERAND && second_op_type != NUMBER)
			{
				/*insert to table*/
				first_word_inst(opcode,first_op_type,second_op_type, one_operand, word_array);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, NULL, num_of_line);
				(*IC)++;

				isLabel = word_by_type(first_op_type, first_operand, word_array,regSrc);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, first_operand, num_of_line);
				(*IC)++;

				isLabel = word_by_type(second_op_type, second_operand, word_array,regSrc);
				insert_to_code_word(code_word_t, IC, word_array, isLabel, second_operand, num_of_line);
				(*IC)++;
			}
			else{   /*the opcode not match the operands*/				
				/*ERROR*/
				fprintf(stderr, "ERROR in line %d - The opcode did not match the operands\n", num_of_line);
				return FALSE;
			}
			break;

		case OP_NONE:
			/*ERROR*/
			fprintf(stderr, "ERROR in line %d - Invalid instruction\n", num_of_line);
			return FALSE;
	}

	free(first_operand);
	free(second_operand);		

	return TRUE;
}


bool insert_to_code_word(code_word **head, long *new_IC, int binaryArray[], bool isLabel, char *label, int num_of_line)
{
	int i;
	code_word *new_node = (code_word *)malloc(sizeof(code_word));
	if (new_node == NULL) {
		/*ERROR*/
		fprintf(stderr, "ERROR in line %d - Memory allocation failed\n", num_of_line);
		return FALSE;
	}
	
	/* Initialize the new node with the given values*/
	new_node->IC = *new_IC;
	for (i = 0; i < MAX_BITS; i++) {
		new_node->data[i] = binaryArray[i];
	}

	if(isLabel){
		strcpy(new_node->label, label);
	}
	else{
		strcpy(new_node->label, "");
	}

	new_node->next = NULL; 

	if (*head == NULL) 
	{
		*head = new_node;
	} 
	else {
		code_word *current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}

	return TRUE;
}


bool word_by_type(type_op type, char *operand, int word_array[], bool regSrc)
{
	int i;
	if(type == LABEL)
	{
		for (i = 0; i < MAX_BITS; i++){
			word_array[i] = 0;
		}
		return TRUE; /* return TRUE- the type of the operand is label */
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

	return FALSE;
}


