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


/*
long decToBinary(int decimal) 
{
	long binary = 0, base = 1;
	bool isNegative = FALSE;

	if (decimal < 0) {
		isNegative = TRUE;
		decimal = -decimal; /* Make the number positive for easier processing */
	/*}

	while (decimal > 0) {
		binary += (decimal % 2) * base;
		decimal /= 2;
		base *= 10;
	}
	
	
	if (isNegative) {
		printf("binary:%ld\n",binary);
	}
	

    return binary;
}*/

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

/*check if register is valid:*/
bool valid_regi(char *regi)
{
	return strlen(regi)==3 && regi[0] == '@' && regi[1] == 'r' && (0 <= regi[2] && regi[2] >= 7);
}

/*check if label or mcro is valid:*/
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
	return TRUE;
}


/* Function to map a string to the corresponding enum value: */
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
	if(op == NULL || op[0] == '\0') return NO_OPERAND;
	if(isdigit(op)) return NUMBER;
	if(valid_regi(op)) return REGISTER;
	if(valid_label_mcro(op)) return LABEL;
	return INCORRECT;
}








