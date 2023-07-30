#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "variables.h"
#include "utils.h"
#include "tables.h"


bool firstPass(FILE *file);

bool processing_line(char curr_line[MAX_LINE+2], symbol_t *symbol_table/*,kidudNetonim_t *kidudNetonim_table, long  dc*/);
