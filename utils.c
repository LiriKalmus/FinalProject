#include "utils.h"

char* concat_str(char *s1, char *s2)
{
	char *str = malloc(strlen(s1) + strlen(s2) + 1); 
	if(str == NULL){
		fprintf(stderr, "memory cannot be allocated!!\n");
		return NULL;
	}
	strcpy(str, s1);
	strcat(str, s2);
	return str;
}



