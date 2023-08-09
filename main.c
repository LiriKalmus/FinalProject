#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"

int main(int argc, char **argv)
{
	int i;

	if(argc < 2){
		fprintf(stderr, "No file has been entered!\n");
		exit(0);
	}
	
	for(i=1; i<argc; i++){
		file_processing(argv[i]);
	}
	return 1;
}
