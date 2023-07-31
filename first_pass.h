#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "variables.h"
#include "utils.h"
#include "tables.h"


bool firstPass(FILE *file, long *IC, long *DC, symbol_t *symbol_table, data_img *data_table);

bool processing_line(char curr_line[MAX_LINE+2], long *IC, long *DC, symbol_t *symbol_table, data_img *data_table);

bool add_data_to_data_img(char curr_line[MAX_LINE+2], int *position, char *directive, data_img *data_table);

data_img* insert_data(data_img *data_table, long data);
