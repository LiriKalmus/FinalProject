#include "utils.h"
    
char** separate_line(char *line)
{
	char **words = NULL; 
    	int array_size = 0;
	char *word;
	char *copy_line = malloc(strlen(line) + 1);
	if(copy_line == NULL){
		fprintf(stderr, "memory cannot be allocated!!\n");
		return NULL;
	}

	strcpy(copy_line, line);
	/*Use strtok to get the first word*/
	word = strtok(copy_line, " ");

	/*Loop through the remaining words*/
	while (word != NULL) {
		array_size++;
		/*Dynamically allocate memory for each word and store it in the array*/
		words = realloc(words, array_size * sizeof(char*));
		words[array_size - 1] = malloc(strlen(word) + 1); 
		strcpy(words[array_size - 1], word);
		word = strtok(NULL, " ");
	}
	/* add a last elemnt to be null*/
	array_size++;
	words = realloc(words, array_size * sizeof(char*));
	words[array_size-1] = NULL;
	free(copy_line);
	return words;
}


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

bool valid_label(char *word)
{
	word[strlen(word)-1] = '\0';
	
	printf("IS A LABEL:%s\n",word);

	/*check if label is valid:*/
	return TRUE;

}

bool valid_directive(char *word)
{
        memmove(word, word + 1, strlen(word));
	printf("IS A directive:%s\n",word);

	/*check if directive is valid:*/
	
	return TRUE;

}

