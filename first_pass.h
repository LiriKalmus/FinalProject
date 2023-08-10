#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "variables.h"
#include "utils.h"
#include "tables.h"


bool firstPass(FILE *file, long *IC, long *DC, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t);

bool processing_line(char curr_line[MAX_LINE+2], long *IC, long *DC, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t, int num_of_line);

bool processing_data(char curr_line[MAX_LINE+2], int *position, char *directive, data_img **data_table, long *DC, int num_of_line);

bool processing_string(char curr_line[MAX_LINE+2], int *position, char *directive, data_img **data_table, long *DC, int num_of_line);

bool insert_to_data_img(data_img **head, long *new_DC, int binaryArray[], int num_of_line);

bool add_labels_to_symbol_table(char curr_line[MAX_LINE+2], int *position, int num_of_line, symbol_t *symbol_table);

bool processing_instruction(char curr_line[MAX_LINE+2], int *position, char *word, code_word **code_word_t, long *IC, int num_of_line);

bool insert_to_code_word(code_word **head, long *new_IC, int binaryArray[], bool isLabel, char *label, int num_of_line);

bool word_by_type(type_op type, char *operand, int word_array[], bool regSrc);

