#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include "ssu_shell.h"
#include "ssu_function.h"

void shellUsage(void){
	printf("too many argument. plz input less or equal to 10 arguments.\n");
}

void ssu_shell(){
	int argc=0;
	char *argv[10];
	int p_argv[10] = {0, };
	char prompt[] = "20142404 $ ";
	char buf[BUFSIZE] = {0, };
	char buf2[BUFSIZE] = {0, };
	char tmp[BUFSIZE] = {0, };
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
	
	printf("asdf\n");
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
		if(argc < 10)
			argv[argc] = str;
	}

	if(argc >= 10){
		// err : print Shell Usage
		shellUsage();
	}
	else{
		 for(i=0; i<2; i++){
		 	if(i==1 && argv[0] != NULL){
				system(buf2);
			}
			else if(!strcmp(cmdList[i].cmd, argv[0])){
				cmdList[i].func(argc, argv);
				break;
			}
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

	printf("time : %ld.%ld\n", sec, micro);

	while(getchar() != '\n');
	fflush(stdout);
}
