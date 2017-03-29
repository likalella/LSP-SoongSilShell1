#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include "ssu_shell.h"
#include "ssu_function.h"
#include "ssu_usage.h"

void ssu_shell(){
	int argc=0;
	char *argv[50];
	char buf[2600] = {0, };
	char buf2[2600] = {0, };
	char *str;
	struct ssu_function cmdList[] = {
		{"ssu_sed", ssu_sed},
		{NULL}
	};
	struct timeval startTime;
	struct timeval endTime;
	long sec;
	long micro;
	int opt;
	int i, j;
	
	printf("20142404 $ ");
	scanf("%[^\n]s", buf);
	gettimeofday(&startTime, NULL);
	
	if(*buf == '\0'){
		while(getchar() != '\n');
		return;
	}

	strcpy(buf2, buf);

	str = strtok(buf, " \t");
	argv[0] = str;
	while(1){
		argc++;
		str = strtok(NULL, " \t");
		if(str == NULL) break;
		argv[argc] = str;
	}

	

	 for(i=0; i<2; i++){
	 	if(i==1 && argv[0] != NULL){
			system(buf2);
		}
		else if(!strcmp(cmdList[i].cmd, argv[0])){
			cmdList[i].func(argc, argv);
			break;
		}
	}

	
	//memory free zone
	memset(buf, '\0', 1);
	memset(buf2, '\0', 1);

	gettimeofday(&endTime, NULL);
	
	sec = (endTime.tv_sec - startTime.tv_sec);
	micro = (endTime.tv_usec - startTime.tv_usec);

	if(micro >= 1000000){
		sec += 1;
		micro -= 1000000;
	}
	if(micro < 0){
		sec -= 1;
		micro += 1000000;
	}

	while(micro >= 1000)
		micro /= 10;

	while(micro < 100)
		micro *= 10;

	printf("\n");
	printf("time : %ld.%ld\n", sec, micro);

	while(getchar() != '\n');
	fflush(stdout);
}
