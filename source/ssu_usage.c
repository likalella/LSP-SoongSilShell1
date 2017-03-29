#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void shellUsage(void){
	printf("Usage : too many argument. plz input less or equal to 10 arguments.\n");
}

void sedUsage(void){
	printf("Usage : ssu_sed [TARGET] [SRC_STR] [DEST_STR] [OPTION]\n");
	printf("[OPTION] : -b\n");
	printf("           -u [USERNAME]\n");
	printf("           -i [STRING]\n");
	printf("           -e [STRING]\n");
	printf("           -d [DEPTH]\n");
	printf("           -p\n");
	printf("           -s\n");
	printf("           -P [PATHNAME]\n");
	return;
}
