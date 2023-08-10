#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "variables.h"


/*
function that get a line and a position and skip all the whitespaces

@param line
@param position

*/
void skip_whitespace(char* line, int* position);



/*
function that get a line and a position and return the next word in the line

@param line
@param position
@return *char - word

*/
char* get_next_word(char* line, int* position);



/*
function that get a line and a position and a number and return if the next char in the line is a number

@param line
@param position
@param number
@return whether is the char is a number

*/
bool get_next_num(char* line, int* position, int *number);



/*
function that check if the end of the line in legal

@param curr_line
@param position
@param num_of_line
@return true if legal, otherwise false

*/
bool suffix_line(char *curr_line, int *position, int num_of_line);



/*
function that get decimal number, array and the array length and update the array to hold the digits of the binary's number

@param decimal
@param binaryArray[]
@param length

*/
void decToBinary(int decimal, int binaryArray[], int length);



char* concat_str(char *s1, char *s2);



/*
check if register is valid

@param regi
@return whether the register is valid or not
 
*/
bool valid_regi(char *regi);



/*
check if label or mcro is valid

@param word
@return whether the word\lablel is valid or not, according to the rules in the course booklet

*/
bool valid_label_mcro(char *word);



/* 
Function to map a string to the corresponding enum value

@param str
@return inst_op

*/
inst_op stringToEnum(char* str);



/* 
function that recieve an operator and reutrn the operator type

@param op
@return type_op (INCORRECT,LABEL,REGISTER,NO_OPERAND,NUMBER)

*/
type_op get_type_op(char* op);



/* 
function that handle the first instruction word in every line

@param opcode
@param first_op_type
@param second_op_type
@param one_operand - a bool that declare wheter we have one or two operands (it helps us to know where and how to decode the operand\s)
@param word_array[]
@return the decode of the word

*/
void first_word_inst(int opcode, int first_op_type, int second_op_type, bool one_operand, int word_array[]);



/* 
function that send the registers to decoding and save it in the right order 

@param src
@param dst
@param word_array[]

*/
void word_regi(int src, int dst, int word_array[]);



/* 
function that send the number to decoding and save it 

@param number
@param word_array[]

*/
void word_number(int number, int word_array[]);



