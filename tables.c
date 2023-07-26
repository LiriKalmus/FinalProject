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
