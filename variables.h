#ifndef _VARIABLES_H
#define _VARIABLES_H


#define MAX_LINE 80
#define MAX_LABEL 31

/*enables boolean functions*/
typedef enum Bool{
	FALSE=0,
	TRUE=1
} bool;


typedef enum symbol_type {
	code_type,
	date_type,
	external_type
} symbol_type;

typedef struct code_word{
	unsigned int ARE: 2;
	/*int type; // 3 types: opcode, register, value.*/
	
	union bits{
		struct data{
			unsigned int value: 10;
		} data;
		struct opcode{
			unsigned int mion_dest: 3;
			unsigned int opcode: 4;
			unsigned int mion_source: 3;
		} opcode;
		struct regi{
			unsigned int reg_src: 5;
		        unsigned int dst_reg: 5;
		} regi;
	} bits;
} code_word;


typedef struct code_file{
	code_word word;
	struct code_file *next;
} code_file;

typedef struct data_img{
	long bin_data;
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
	NO_OPERAND,
	NUMBER,
	REGISTER,
	LABEL
	INCORRECT
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
