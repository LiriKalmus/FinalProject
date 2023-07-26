#include "assembler.h"


bool file_processing(char *fileName)
{
	char *am_fileName;
	FILE *am_file;

 	if(!pre_assembly(fileName)){
		fprintf(stderr, "The macros did not spread properly in %s.as! \n", fileName);
		return FALSE;
	}

	am_fileName = concat_str(fileName, ".am");
	am_file = fopen(am_fileName, "r");
	/* if unable to open the file: */
	if(am_file == NULL){
		fprintf(stderr, "Unable to open %s.am! \n", fileName);
		free(am_fileName);
        	fclose(am_file);
		return FALSE;
	}
	
	if(!firstPass(am_file)){
		printf("FIRST PASS FAILED");
	}
	
	free(am_fileName);
       	fclose(am_file);
	return TRUE;
}
