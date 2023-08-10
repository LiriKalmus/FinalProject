#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tables.h"


mcro_t* createTable_mcro()
{
	mcro_t* tp = (mcro_t*)malloc(sizeof(mcro_t));
	if(tp)
	{
		tp->values = NULL;
		tp->num_mcros =0;
	}
	return tp;
}


void addToTable_mcro (mcro_t* tp, const char* mcro_name, const char* mcro_definition)
{
	mcro *new_values;
	int mcro_definition_length;
	if(!tp || !mcro_name || !mcro_definition)
	{
		return;
	}

	new_values = (mcro*)realloc(tp->values, (tp->num_mcros+1)*sizeof(mcro));
	if(!new_values)
	{
		fprintf(stderr, "memory cannot be allocated!!\n");
		return ;
	}

	tp->values = new_values;
	tp->values[tp->num_mcros].mcro_name = malloc(strlen(mcro_name) + 1); 
	if(!tp->values[tp->num_mcros].mcro_name)
	{
		fprintf(stderr, "memory cannot be allocated!!\n");
		return;
	}
	strcpy(tp->values[tp->num_mcros].mcro_name,mcro_name);
	
	mcro_definition_length = strlen(mcro_definition);
	tp->values[tp->num_mcros].mcro_definition=(char*)malloc((mcro_definition_length+1)*sizeof(char));

	if(!tp->values[tp->num_mcros].mcro_definition)
	{
		fprintf(stderr, "ERROR - memory cannot be allocated!!\n");		
		return;
	}

	strcpy(tp->values[tp->num_mcros].mcro_definition,mcro_definition);
	tp->num_mcros++;
	
}

const char* getFromTable_mcro (const mcro_t* tp, const char* mcro_name)
{
	int i;
	if(!tp || !mcro_name)
	{
		return NULL;
	}
	for(i=0;i<(tp->num_mcros);i++){
		if(strcmp(tp->values[i].mcro_name,mcro_name)==0)
		{
			return tp->values[i].mcro_definition;
		}
	}
	return NULL;
}


void freeMcro_t (mcro_t* tp)
{
	int i;
	if(!tp)
	return;


	for(i=0;i<tp->num_mcros;i++)
	{
		free(tp->values[i].mcro_name);
		free(tp->values[i].mcro_definition);
	}
	free(tp->values);
	free(tp);
}


symbol_t *createTable_symbol()
{
	symbol_t* tp = (symbol_t*)malloc(sizeof(symbol_t));
	if(tp)
	{
		tp->values = NULL;
		tp->num_symbols =0;
	}
	return tp;
}

int search_symbol(symbol_t* tp, const char* symbol_name)
{
	int i;
	for(i=0; i<tp->num_symbols; i++){
		if(strcmp(tp->values[i].symbol_name, symbol_name) == 0){
			return i;
		}
	}	
	return -1;
}

bool addToTable_symbol (symbol_t* tp, const char* symbol_name, long *address, symbol_type type)
{
	symbol* new_symbol;
		
	if(!tp || !symbol_name || tp->num_symbols<0)
	{
		return FALSE;
	}

	if(search_symbol(tp, symbol_name) != -1) return FALSE; 

	new_symbol = (symbol*)malloc(sizeof(symbol));
	if(!new_symbol)
	{
		fprintf(stderr, "memory cannot be allocated!!\n");
		return FALSE;
	}

	new_symbol->symbol_name=(char*)malloc((strlen(symbol_name)+1)*sizeof(char));
	
	if(!new_symbol->symbol_name)
	{
		free(new_symbol);
		fprintf(stderr, "memory cannot be allocated!!\n");
		return FALSE;
	}


	strcpy(new_symbol->symbol_name,symbol_name);

	if(address == NULL){
		new_symbol->address = 0;
	}
	else{
		new_symbol->address = *address;
	}

	new_symbol->type = type;


	tp->values = (symbol*)realloc(tp->values , (tp->num_symbols+1)*sizeof(symbol));
	if(!tp->values)
	{
		free(new_symbol->symbol_name);
		free(new_symbol);
		return FALSE;
	}
	

	tp->values[tp->num_symbols] = *new_symbol;
	tp->num_symbols++;

	free(new_symbol);
	return TRUE;
}


symbol* getFromTable_symbols (symbol_t* tp, const char *symbol_name)
{
	int i;

	if(!tp || !symbol_name)
	{
		return NULL;
	}
	for (i=0;i<tp->num_symbols;i++)
	{
		if(strcmp(tp->values[i].symbol_name, symbol_name)==0)
		{
		return &(tp->values[i]);
		}

	}
	return NULL;
}


void freeTable_symbol(symbol_t* tp) {
	int i=0;
	if (tp == NULL) {
		return;
	}
	for ( i= 0; i < tp->num_symbols; i++) {
		free(tp->values[i].symbol_name);
	}
	free(tp->values);

	free(tp);
}



void free_data_img(data_img **head) {
    data_img* current = *head;
    data_img* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    *head = NULL;
}


void free_code_word(code_word **head) {
    code_word* current = *head;
    code_word* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    *head = NULL;
}

void update_adresses(code_word **head_c, data_img **head_d, symbol_t* tp)
{
	code_word* current_c = *head_c;
	data_img* current_d = *head_d;
	long IC = 0;
	int i;

	while (current_c != NULL) {
		current_c->IC += START_MEMORY;
		IC = current_c->IC;
		current_c = current_c->next;
	}

	while (current_d != NULL) {
		current_d->DC += IC + 1;
		current_d = current_d->next;
	}

	for ( i= 0; i < tp->num_symbols; i++) 
	{
		if(tp->values[i].type == CODE_TYPE){
			tp->values[i].address += START_MEMORY;
		}
		else if(tp->values[i].type == DATA_TYPE){
			tp->values[i].address += IC + 1;
		}
	}	
}
