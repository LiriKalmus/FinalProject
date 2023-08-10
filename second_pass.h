#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "variables.h"
#include "utils.h"
#include "tables.h"



/**
function that check if the second pass ended succsesfully or not

@param fileName
@param file
@param symbol_table
@param data_table
@param code_word_t
@param IC
@param DC
@return true if second Pass succeeded, otherwise false

*/
bool secondPass(char *fileName, FILE *file, symbol_t *symbol_table, data_img **data_table, code_word **code_word_t, long *IC, long *DC);



/**
function that process each line and return true if succeeded

@param curr_line[MAX_LINE+2]
@param symbol_table
@param num_of_line
@return true if processing_line succeeded, otherwise false

*/
bool processing_line2(char curr_line[MAX_LINE+2], symbol_t *symbol_table, int num_of_line);




/**
function that pass over the the symbols table and print into the ent file the entry symbols with their addresses 

@param fileName
@param symbol_table
@return true if the file has been made successfully, otherwise false

*/
bool write_ent_file(char *fileName, symbol_t *symbol_table);




/**
function that called when we are decoding an external label, when its happend we print the label with the address that the label appears

@param file
@param symbol_name
@param address

*/
void write_ext_file(FILE *file, char *symbol_name, long address);



/**
function that decoding the labels that we can know there's address only after the first pass

@param file
@param symbol_table
@param code_word_t
@param num_of_line
@return true if succeeded

*/
bool decode_labeles(FILE *file, symbol_t *symbol_table, code_word **code_word_t, int num_of_line);



/**
function that receive an array that hold 0/1 numbers (binary) and translate it into Base64

@param binaryArray[]
@param start - represent the start place that we need to start translate
@param end
@return the update array

*/
char binaryToBase64Char(int binaryArray[], int start, int end);



/**
function that create the object file

@param fileName
@param data_table
@param code_word_t
@param code_word_t
@param IC
@param DC
@return true if succeeded

*/
bool write_ob_file(char *fileName, data_img **data_table, code_word **code_word_t, long *IC, long *DC);
