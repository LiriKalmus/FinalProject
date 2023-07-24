#include "utils.h"

char* concat_fileName(char *fileName, char *suffix)
{
	char *fullName = malloc(strlen(fileName) + strlen(suffix) + 1); 
	if(fullName == NULL){
		fprintf(stderr, "memory cannot be allocated!!\n");
		return NULL;
	}
	strcpy(fullName, fileName);
	strcat(fullName, suffix);
	return fullName;
}



