#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "variables.h"
#include "utils.h"
#include "tables.h"

/**
function that check if the first pass ended succsesfully or not

@param file
@param IC
@param DC
@param symbol_table
@param data_table
@param code_word_t
@return true if first Pass succeeded, otherwise false

*/
bool firstPass(FILE *file, long *IC, long *DC, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t);



/**
function that process each line and return true if succeeded

@param curr_line[MAX_LINE+2]
@param IC
@param DC
@param symbol_table
@param data_table
@param code_word_t
@param num_of_line - because we need to write in which line there is an error
@return true if processing_line succeeded, otherwise false

*/
bool processing_line(char curr_line[MAX_LINE+2], long *IC, long *DC, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t, int num_of_line);



/**
function that process each data line (list of numbers) and return true if succeeded

@param curr_line[MAX_LINE+2]
@param position - the current place in the line
@param directive - 
@param data_table
@param DC
@param num_of_line
@return true if processing_data succeeded, otherwise false

*/
bool processing_data(char curr_line[MAX_LINE+2], int *position, char *directive, data_img **data_table, long *DC, int num_of_line);



/**
function that process each string line (one string in "") and return true if succeeded

@param curr_line[MAX_LINE+2]
@param position
@param directive 
@param data_table
@param DC
@param num_of_line
@return true if processing_string succeeded, otherwise false

*/
bool processing_string(char curr_line[MAX_LINE+2], int *position, char *directive, data_img **data_table, long *DC, int num_of_line);



/**
function that receive the data_img head and add the new data into the linked list

@param head
@param new_DC
@param binaryArray[] 
@param num_of_line
@return true if the insert to data_img succeeded, otherwise false

*/
bool insert_to_data_img(data_img **head, long *new_DC, int binaryArray[], int num_of_line);



/**
function that receive a line and the current position, find the label, and add it to the symbol_table

@param curr_line[MAX_LINE+2]
@param position
@param num_of_line 
@param symbol_table
@return true if the insert to symbol_table succeeded, otherwise false

*/
bool add_labels_to_symbol_table(char curr_line[MAX_LINE+2], int *position, int num_of_line, symbol_t *symbol_table);



/**
function that receive a line and the current position, go over the instruction and check if the line is legal

@param curr_line[MAX_LINE+2]
@param position
@param word 
@param code_word_t
@param IC
@param num_of_line
@return true if the line is legal, otherwise false

*/
bool processing_instruction(char curr_line[MAX_LINE+2], int *position, char *word, code_word **code_word_t, long *IC, int num_of_line);



/**
function that receive the code word head and add the new data into the linked list

@param head
@param new_IC
@param binaryArray[] 
@param isLabel
@param label
@param num_of_line
@return true if the insert ended successfully, otherwise false

*/
bool insert_to_code_word(code_word **head, long *new_IC, int binaryArray[], bool isLabel, char *label, int num_of_line);



/**
function that receive operand type and according to that we know how to decode the line

@param type
@param operand
@param word_array[] 
@param regSrc - a flag to know if the register is the first or the second operand
@return true if the process went right, otherwise false

*/
bool word_by_type(type_op type, char *operand, int word_array[], bool regSrc);

