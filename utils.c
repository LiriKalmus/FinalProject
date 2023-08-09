#include "utils.h"



void skip_whitespace(char* line, int* position) 
{    
	/* Skip leading whitespace */
	while (line[*position] && isspace(line[*position])) {
		(*position)++;
	}
}
 


char* get_next_word(char *line, int *position)
{
	char *word_start, *result;
	int i, word_length = 0;

	skip_whitespace(line, position);

	/* Determine the length of the first word */
	word_start = line + *position;
	while (line[*position] && !isspace(line[*position])) {
		word_length++;
		(*position)++;
	}

	/* Allocate memory for the extracted word */
	result = (char*)malloc((word_length + 1) * sizeof(char));
	if (result == NULL) {
		fprintf(stderr, "Memory allocation failed!\n");
		return NULL;
	}

	/* Copy the first word into the result array*/
	for (i = 0; i < word_length; i++) {
		result[i] = word_start[i];
	}
	result[i] = '\0'; /* Null-terminate the extracted word*/
	

	return result;
}



bool get_next_num(char* line, int* position, int* number)
{
	*number = atof(&line[*position]);
	
	/* if is a negative number: */
	if(line[*position] == '-' || line[*position] == '+')
		(*position)++;

	/* if the parameter is not a number: */
	if(!isdigit(line[*position]))
		return FALSE;

	while(isdigit(line[*position]))
		(*position)++;

	/* if there are spaces or tabs at the ending- so we ignore them: */	
	skip_whitespace(line, position);

	return TRUE;
}




void decToBinary(int decimal, int binaryArray[], int length) 
{
	int i, carry = 1;
	bool isNegative = FALSE;
	for (i = 0; i < length; i++) {
		binaryArray[i] = 0;
	}

	if (decimal < 0) {
		isNegative = TRUE;
		decimal = -decimal; /* Make the number positive for easier processing */
	}

	for (i = length-1; i >= 0; i--){
		binaryArray[length-1-i] = (decimal >> i) & 1; 
	}

	/* If the number is negative, invert all the bits and add 1: */
	if (isNegative)
	{
		for (i = length-1; i >= 0; i--)
		{
			binaryArray[i] = !binaryArray[i]; 
			binaryArray[i] += carry; 
			carry = binaryArray[i] / 2; 
			binaryArray[i] %= 2; 
		}
	}
}

char* concat_str(char *s1, char *s2)
{
	char *result = malloc(strlen(s1) + strlen(s2) + 1); 
	if(result == NULL){
		fprintf(stderr, "memory cannot be allocated!!\n");
		return NULL;
	}
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}



bool valid_regi(char *regi)
{
	return strlen(regi)==3 && regi[0] == '@' && regi[1] == 'r' && (0 <= regi[2] && regi[2] >= 7);
}


bool valid_label_mcro(char *word)
{
	if(strlen(word) <= MAX_LABEL && isalpha(word[0])){
		while (*word) {
			if (!isalpha(*word) && !isdigit(*word)) {
				return FALSE; /* Return FALSE if a non-letter, non-digit character is found */
			}
			word++; 
		}
	}
	else
		return FALSE;
	return TRUE;
}



inst_op stringToEnum(char* str) 
{
    if (strcmp(str, "mov") == 0) return OP_MOV;
    if (strcmp(str, "cmp") == 0) return OP_CMP;
    if (strcmp(str, "add") == 0) return OP_ADD;
    if (strcmp(str, "sub") == 0) return OP_SUB;
    if (strcmp(str, "not") == 0) return OP_NOT;
    if (strcmp(str, "clr") == 0) return OP_CLR;
    if (strcmp(str, "lea") == 0) return OP_LEA;
    if (strcmp(str, "inc") == 0) return OP_INC;
    if (strcmp(str, "dec") == 0) return OP_DEC;
    if (strcmp(str, "jmp") == 0) return OP_JMP;
    if (strcmp(str, "bne") == 0) return OP_BNE;
    if (strcmp(str, "red") == 0) return OP_RED;
    if (strcmp(str, "prn") == 0) return OP_PRN;
    if (strcmp(str, "jsr") == 0) return OP_JSR;
    if (strcmp(str, "rts") == 0) return OP_RTS;
    if (strcmp(str, "stop") == 0) return OP_STOP;
    return OP_NONE;
}


type_op get_type_op(char* op)
{
	int i;

	if(op == NULL || op[0] == '\0') return NO_OPERAND;
	if(valid_regi(op)) return REGISTER;
	if(valid_label_mcro(op)) return LABEL;

	for (i = 0; op[i]; i++) {
		if(op[0] == '-' || op[0] == '+') continue;
		if (!isdigit(op[i])) {
			return INCORRECT;
		}
	}
	return NUMBER;
}



void first_word_inst(int opcode, int first_op_type, int second_op_type, bool one_operand, int word_array[])
{
	int i;
	int binary_opcode[MAX_OP_BINARY], binary_mion[MAX_MION_BINARY];

	for (i = 0; i < MAX_BITS; i++) {
		word_array[i] = 0;
	}

	decToBinary(opcode, binary_opcode, MAX_OP_BINARY);
	for (i = 0; i < MAX_OP_BINARY; i++) {
		word_array[3+i] = binary_opcode[i];
	}

	decToBinary(first_op_type, binary_mion, MAX_MION_BINARY);
	if(one_operand)
	{
		for (i = 0; i < MAX_MION_BINARY; i++) {
			word_array[7+i] = binary_mion[i];

		}
	}
	else
	{
		for (i = 0; i < MAX_MION_BINARY; i++) {
			word_array[i] = binary_mion[i];
		}
		
		decToBinary(second_op_type, binary_mion, MAX_MION_BINARY);
		for (i = 0; i < MAX_MION_BINARY; i++) {
			word_array[7+i] = binary_mion[i];
		}
	}
}



void word_regi(int src, int dst, int word_array[])
{
	int i;
	int binary_regi[MAX_REGI_BINARY];

	for (i = 0; i < MAX_BITS; i++) {
		word_array[i] = 0;
	}

	decToBinary(src, binary_regi, MAX_REGI_BINARY);
	for (i = 0; i < MAX_REGI_BINARY; i++) {
		word_array[i] = binary_regi[i];
	}
	
	decToBinary(dst, binary_regi, MAX_REGI_BINARY);
	for (i = 0; i < MAX_REGI_BINARY; i++) {
		word_array[5+i] = binary_regi[i];
	}
}



void word_number(int number, int word_array[])
{
	int i;
	int binary_num[MAX_NUM_BINARY];

	for (i = 0; i < MAX_BITS; i++) {
		word_array[i] = 0;
	}

	decToBinary(number, binary_num, MAX_NUM_BINARY);
	for (i = 0; i < MAX_NUM_BINARY; i++) {
		word_array[i] = binary_num[i];
	}
}


					

