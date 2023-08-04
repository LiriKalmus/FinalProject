#ifndef _VARIABLES_H
#define _VARIABLES_H


#define MAX_LINE 80
#define MAX_LABEL 31
#define MAX_BITS 12
#define MAX_OP_BINARY 4
#define MAX_MION_BINARY 3
#define MAX_REGI_BINARY 5
#define MAX_NUM_BINARY 10
#define START_MEMORY 100



/*enables boolean functions*/
typedef enum Bool{
	FALSE=0,
	TRUE=1
} bool;


typedef enum symbol_type {
	CODE_TYPE,
	DATA_TYPE,
	EXTERNAL_TYPE,
	ENTRY_TYPE
} symbol_type;

typedef struct code_word{
	long IC;
	char label[MAX_LABEL];
	int data[MAX_BITS];
	struct code_word *next;
} code_word;

typedef struct data_img{
	long DC;
	int data[MAX_BITS];
	struct data_img *next;
} data_img;

typedef enum inst_op{
	OP_MOV,
	OP_CMP,
	OP_ADD,
	OP_SUB,
	OP_NOT,
	OP_CLR,
	OP_LEA,
	OP_INC,
	OP_DEC,
	OP_JMP,
	OP_BNE,
	OP_RED,
	OP_PRN,
	OP_JSR,
	OP_RTS,
	OP_STOP,
	OP_NONE
} inst_op;

typedef enum type_op{
	NO_OPERAND = 0,
	NUMBER = 1,
	LABEL = 3,
	REGISTER = 5,
	INCORRECT = -1
} type_op;	



/*
typedef enum symbol_t{
    ENTRY_SYMBOL = 0,
    DATA_SYMBOL, 
    EXTERN_SYMBOL, 
    CODE_SYMBOL
} instruction_lookup_item;


t

	struct instruction_lookup_item *curr_item;
	for (curr_item = instructions_lookup_table; curr_item->name != NULL; curr_item++) {
		if (strcmp(curr_item->name, name) == 0) {
			return curr_item->value;
		}
	}
	return NONE_INST;*/
#endif
