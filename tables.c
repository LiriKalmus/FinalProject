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
return;
}

tp->values = new_values;
strcpy(tp->values[tp->num_mcros].mcro_name,mcro_name);

mcro_definition_length = strlen(mcro_definition);
tp->values[tp->num_mcros].mcro_definition=(char*)malloc((mcro_definition_length+1)*sizeof(char));

if(!tp->values[tp->num_mcros].mcro_definition)
{
return ;
}

strcpy(tp->values[tp->num_mcros].mcro_definition,mcro_definition);
tp->num_mcros++;
printf("add: %s\n", tp->values[0].mcro_name);
}

const char* getFromTable_mcro (const mcro_t* tp, const char* mcro_name)
{
/*if(!tp || !mcro_name)
{
printf("111\n");
return NULL;
}*/


int i;
for(i=0;i<(tp->num_mcros);i++)
{printf("----%s\n", tp->values[i].mcro_definition);
	if(strcmp(tp->values[i].mcro_name,mcro_name)==0)
	{
		printf("++++g%s\n", tp->values[i].mcro_definition);
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
free(tp->values[i].mcro_definition);
}
free(tp->values);
free(tp);

}




