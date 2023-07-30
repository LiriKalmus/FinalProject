#ifndef _VARIABLES_H
#define _VARIABLES_H


#define MAX_LINE 80

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


typedef enum inst_op {
	inst_mov,
	inst_cmp,
	inst_add,
	inst_sub,
	inst_not,
	inst_clr,
	inst_lea,
	inst_inc,
	inst_dec,
	inst_jmp,
	inst_bne,
	inst_red,
	inst_prn,
	inst_jsr,
	inst_rts,
	inst_stop
} inst_op;




typedef struct code_word {
	unsigned int ARE: 2;
	unsigned int mion_dest: 3;
	unsigned int opcode: 4;
	unsigned int mion_source: 3;

} code_word;


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
