#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_shell.h"

int main(void){

	system("clear");

	while(1){
		ssu_shell();	
	}
	return 0;
}
