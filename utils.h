#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "variables.h"

char** separate_line(char *line);
char* concat_str(char *s1, char *s2);

bool valid_label(char *word);
bool valid_directive(char *word);
