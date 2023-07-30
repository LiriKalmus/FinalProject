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
		fprintf(stderr, "memory cannot be allocated!!\n");		
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

void addToTable_symbol (symbol_t* tp, const char* symbol_name, int address, symbol_type type)
{
	symbol* new_symbol;

	if(!tp || !symbol_name || tp->num_symbols<0)
	{
		return;
	}

	new_symbol = (symbol*)malloc(sizeof(symbol));
	if(!new_symbol)
	{
		fprintf(stderr, "memory cannot be allocated!!\n");
		return;
	}

	new_symbol->symbol_name=(char*)malloc((strlen(symbol_name)+1)*sizeof(char));
	
	if(!new_symbol->symbol_name)
	{
	free(new_symbol);
	fprintf(stderr, "memory cannot be allocated!!\n");
	}

strcpy(new_symbol->symbol_name,symbol_name);
new_symbol->address = address;
new_symbol->type = type;

tp->values = (symbol*)realloc(tp->values , (tp->num_symbols+1)*sizeof(symbol));
if(!tp->values)
{
	free(new_symbol->symbol_name);
	free(new_symbol);
	return;
}

tp->values[tp->num_symbols] = *new_symbol;
tp->num_symbols++;

free(new_symbol);

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

void printSymbolTable(symbol_t* tp)
{
  int i;
    if (tp == NULL || tp->values == NULL || tp->num_symbols == 0)
    {
        printf("Symbol table is empty.\n");
        return;
    }

    printf("Symbol table contents:\n");
    printf("---------------------\n");
  
    for (i = 0; i < tp->num_symbols; i++)
    {
        symbol* sym = &(tp->values[i]);
        printf("Symbol Name: %s\n", sym->symbol_name);
        printf("Address: %d\n", sym->address);
        printf("type: %d\n", sym->type);

        printf("---------------------\n");
    }
}





/*kidod netonim*/

/*
void addToKidudNetonim_t(kidudNetonim_t* tp, long new_DC, const char* new_data)
{
    
    char* new_entry = (char*)malloc(strlen(new_data) + 1);
    if (new_entry == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    strcpy(new_entry, new_data);


    char** resized_data = (char**)realloc(tp->data, (new_DC + 1) * sizeof(char*));
    if (resized_data == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        free(new_entry);
        return;
    }


    tp->data = resized_data;
    tp->data[new_DC] = new_entry;
    tp->DC = new_DC;
}

kidudNetonim_t* createKidudNetonim()
{
    kidudNetonim_t* tp = (kidudNetonim_t*)malloc(sizeof(kidudNetonim_t));
    if (tp == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    tp->data = NULL;
    tp->DC = 0;

    return tp;
}


void freeKidudNetonim_t(kidudNetonim_t* tp)
{
    int i;
    for (i = 0; i < tp->DC; i++)
    {
        free(tp->data[i]);
    }
    free(tp->data);
}

*/

