#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "variables.h"

void skip_whitespace(char* line, int* position);

char* get_next_word(char* line, int* position);
bool get_next_num(char* line, int* position, int *number);

void decToBinary(int decimal, int binaryArray[], int length);

char* concat_str(char *s1, char *s2);

bool valid_regi(char *regi);
bool valid_label_mcro(char *word);

inst_op stringToEnum(char* str);

type_op get_type_op(char* op);

void first_word_inst(int opcode, int first_op_type, int second_op_type, bool one_operand, int word_array[]);

void word_regi(int src, int dst, int word_array[]);

void word_number(int number, int word_array[]);

