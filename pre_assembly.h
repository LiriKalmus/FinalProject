#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "variables.h" 
#include "utils.h"
#include "tables.h"



/**
function that go over the AS file, check that the lines are not too long, and create the AM file with the spread mcros

@param fileName
@return true if the the AM file was successfully made

*/
bool pre_assembly(char *fileName);

