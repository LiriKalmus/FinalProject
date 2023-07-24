#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <ctype.h>*/


#include "variables.h" 
#include "utils.h" 
#include "pre_assembly.h" 


bool file_processing(char *fileName);

int main(int argc, char **argv)
{
	int i;

	if(argc < 2){
		fprintf(stderr, "no enter a files!\n");
		exit(0);
	}
	
	for(i=1; i<argc; i++){
		file_processing(argv[i]);
	}
	return 1;
}


bool file_processing(char *fileName)
{
 	if(!pre_assembly_file(fileName)){
		fprintf(stderr, "The macros did not spread properly in %s.as! \n", fileName);
		return FALSE;
	}
	return TRUE;
}
