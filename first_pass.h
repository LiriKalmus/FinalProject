#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "variables.h"
#include "utils.h"
#include "tables.h"


bool firstPass(FILE *file, long *IC, long *DC, symbol_t *symbol_table, data_img **data_table);

bool processing_line(char curr_line[MAX_LINE+2], long *IC, long *DC, symbol_t *symbol_table, data_img **data_table);

bool processing_data(char curr_line[MAX_LINE+2], int *position, char *directive, data_img **data_table, long *DC);

bool insert_to_data_img(data_img **head, long *new_DC, int binaryArray[]);
