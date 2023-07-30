#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "variables.h"



typedef struct mcro
{
char *mcro_name;
char *mcro_definition;

}mcro;

typedef struct mcro_t
{
mcro* values;
int num_mcros;

}mcro_t;


/* 

create an empty mcro table 
@ return the table's pointer

*/
mcro_t* createTable_mcro();



/* 

add a mcro to the table 

*/
void addToTable_mcro (mcro_t* tp, const char* mcro_name, const char* mcro_definition);



/* 

get a mcro definition from the table
return a poiner to the mcro definition 

*/
const char* getFromTable_mcro (const mcro_t* tp, const char* mcro_name);


/* 

free the memory of the mcro table

*/

void freeMcro_t (mcro_t* tp);


/* SYMBOLS TABLE*/



typedef struct symbol
{
	char* symbol_name;
	int address;
	symbol_type type;

}symbol;

typedef struct symbol_t
{
symbol* values;
int num_symbols;

}symbol_t;

/* 

create an empty symbol table 
@ return the table's pointer

*/
symbol_t *createTable_symbol();

/* 

add a symbol to the table 

*/
void addToTable_symbol (symbol_t* tp, const char* symbol_name, int address, symbol_type type);



/* 

get a symbol from the table
return a poiner to the symbol

*/
symbol* getFromTable_symbols (symbol_t* tp, const char *symbol_name);

void printSymbolTable(symbol_t* tp);

/*data\string word

typedef struct kidudNetonim_t
{
	long DC;
	char* data;
	
}kidudNetonim_t;


kidudNetonim_t* createKidudNetonim();
void addToKidudNetonim_t(kidudNetonim_t* tp, long new_DC, const char* new_data);
void freeKidudNetonim_t(kidudNetonim_t* tp);*/


