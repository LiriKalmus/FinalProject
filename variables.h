#ifndef _VARIABLES_H
#define _VARIABLES_H


#define MAX_LINE 80

/*enables boolean functions*/
typedef enum Bool{
	FALSE=0,
	TRUE=1
} bool;
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
