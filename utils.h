#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "variables.h"

void skip_whitespace(char* line, int* position);

char* get_next_word(char* line, int* position);
bool get_next_num(char* line, int* position, int *number);

void decToBinary(int decimal, int binaryArray[]);

char* concat_str(char *s1, char *s2);

bool valid_regi(char *regi);
bool valid_label_mcro(char *word);

inst_op stringToEnum(char* str);
