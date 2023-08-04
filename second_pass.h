#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "variables.h"
#include "utils.h"
#include "tables.h"

bool secondPass(char *fileName, FILE *file, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t);

bool processing_line2(char curr_line[MAX_LINE+2], symbol_t *symbol_table);

bool write_ent_file(char *fileName, symbol_t *symbol_table);

void write_ext_file(FILE *file, char *symbol_name, long address);

bool decode_labeles(FILE *file, symbol_t *symbol_table, code_word **code_word_t);

char binaryToBase64Char(int binaryArray[], int start, int end);

bool write_ob_file(char *fileName, data_img **data_table, code_word **code_word_t);
