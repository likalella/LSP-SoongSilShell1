#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ssu_function.h"
#include "ssu_sed.h"
#include "ssu_shell.h"
#include "ssu_usage.h"
#include "ssu_string.h"

void ssu_sed(int argc, char *argv[]){
	struct sedOption sed_opt;
	struct passwd *u_info;
	int m[10] = {0, };
	char fullpath[BUFSIZE];
	char pathname[BUFSIZE];
	char curpath[BUFSIZE];
	char n;
	char *tmp;
	char *tmp2;
	DIR *dp;
	sed_opt.is_b = 0;
	sed_opt.is_u = 0;
	sed_opt.is_i = 0;
	sed_opt.is_e = 0;
	sed_opt.is_d = 0;
	sed_opt.is_p = 0;
	sed_opt.is_s = 0;
	sed_opt.is_P = 0;
	sed_opt.is_es = 0;
    sed_opt.is_is = 0;
	sed_opt.depth = 0;
	sed_opt.uid=0;
	sed_opt.target = NULL;
	sed_opt.src_str = NULL;
	sed_opt.dest_str = NULL;
	sed_opt.estring = NULL;
	sed_opt.istring = NULL;
	sed_opt.pathname = NULL;
	int len, len2;
	int i, j;
	int p, d;
	int mnum=0;
	int ism = 0;

	// check '\ '
	for(i=1; i<argc-mnum; i++){
		len = ssu_strlen(argv[i]);

		if(argv[i][len-1] == '\\'){
			len2 = ssu_strlen(argv[i+1]);
			tmp = (char *)malloc((len+len2+1) * sizeof(char));
			for(j=0; j < len-1; j++){
				tmp[j] = argv[i][j];
			}
			tmp[len-1] = ' ';
			for(j=0; j<len2; j++){
				tmp[len+j] = argv[i+1][j];
			}
			tmp[len+len2] = '\0';

			argv[i] = tmp;
			for(j=i+2; j<argc-mnum; j++){
				argv[j-1] = argv[j]; 
			}
			mnum++;
		}
	}

	free(tmp);

	if(argc-mnum < 4){
		sedUsage();
		return;
	}
	else if(argc-mnum > 10){
		sedUsage();
		return;
	}

	sed_opt.target = argv[1];
	sed_opt.src_str = argv[2];
	sed_opt.dest_str = argv[3];

	// option check
	for(i=4; i<argc-mnum; i++){
		if(ssu_strcmp(argv[i], "-b") == 0){
			if(sed_opt.is_b == 1){
				sedUsage();
				return;
			}
			sed_opt.is_b = 1;
		}
		else if(ssu_strcmp(argv[i], "-u") == 0){
			if(sed_opt.is_u == 1){
				sedUsage();
				return;
			}
			sed_opt.is_u = 1;
			if(i == argc-mnum-1){
				sedUsage();
				return;
			}
			//sed_opt.username = argv[++i];
			if((u_info = getpwnam(argv[++i])) == NULL){
				sed_opt.uid = -1;
			}
			else{
				sed_opt.uid = u_info->pw_uid;
			}
		}
		else if(ssu_strcmp(argv[i], "-i") == 0){
			if(sed_opt.is_i == i || sed_opt.is_is == 1){
				sedUsage();
				return;
			}
			sed_opt.is_i = 1;
			if(i == argc-mnum-1){
				sedUsage();
				return;
			}
			sed_opt.istring = argv[++i];
		}
		else if(ssu_strcmp(argv[i], "-e") == 0){
			if(sed_opt.is_e == 1 || sed_opt.is_es == 1){
				sedUsage();
				return;
			}
			sed_opt.is_e = 1;
			if(i == argc-mnum-1){
				sedUsage();
				return;
			}
			sed_opt.estring = argv[++i];
		}
		else if(ssu_strcmp(argv[i], "-d") == 0){
			if(sed_opt.is_d == 1){
				sedUsage();
				return;
			}
			sed_opt.is_d = 1;
			if(i == argc-mnum-1){
				sedUsage();
				return;
			}

			if(isdigit(*argv[i+1])){
				sed_opt.depth = atoi(argv[++i]);
			}
			else{
				sedUsage();
				return;
			}
		}
		else if(ssu_strcmp(argv[i], "-p") == 0){
			if(sed_opt.is_p == 1){
				sedUsage();
				return;
			}
			sed_opt.is_p = 1;
		}
		else if(ssu_strcmp(argv[i], "-s") == 0){
			if(sed_opt.is_s == 1){
				sedUsage();
				return;
			}
			sed_opt.is_s = 1;
		}
		else if(ssu_strcmp(argv[i], "-P") == 0){
			if(sed_opt.is_P == 1){
				sedUsage();
				return;
			}
			sed_opt.is_P = 1;
			if(i == argc-mnum-1){
				sedUsage();
				return;
			}

			if(access(argv[++i], F_OK) < 0){
				if(argv[i][0] == '/'){
					printf("lella\n");
					ism = 1;
					d = ssu_strlen(argv[i]);
					printf("d : %d\n", d);
					tmp2 = (char *)malloc(d+1);
					for(j=0; j < d; j++){
						if(argv[i][j] == '/'){
							tmp2[j] = '\0';
							if(access(tmp2, F_OK) < 0){
								mkdir(tmp2, 0755);
							}
						}
						tmp2[j] = argv[i][j];
					}
					tmp2[d] = '\0';
					realpath(tmp2, pathname);
					sed_opt.pathname = pathname;
				}
				else{
					printf("argvaaa : %s\n", argv[i]);
					ism = 1;
					realpath(".", curpath);
					p = ssu_strlen(curpath);
					d = ssu_strlen(argv[i]);

					sed_opt.pathname = (char *)malloc(p+d+2);
					ssu_memset(sed_opt.pathname, '\0', p+d+2);
					ssu_strcpy(sed_opt.pathname, curpath);
					if(curpath[p-1] != '/'){
						sed_opt.pathname[p] = '/';
						ssu_strcpy(&sed_opt.pathname[p+1], argv[i]);
						sed_opt.pathname[p+d+1] = '\0';
					}
					else{
						ssu_strcpy(&sed_opt.pathname[p], argv[i]);
						sed_opt.pathname[p+d] = '\0';
					}
					mkdir(sed_opt.pathname, 0755);
				}
			}
			else{
				printf("aceess ok\n");
				realpath(argv[++i], pathname);
				sed_opt.pathname = pathname;
			}
		}
		else if(ssu_strcmp(argv[i], "-es") == 0){
			if(sed_opt.is_e == 1 || sed_opt.is_es == 1){
				sedUsage();
				return;
			}
			sed_opt.is_es = 1;
			if(i == argc-mnum-1){
				sedUsage();
				return;
			}
			sed_opt.estring = argv[++i];
		}
		else if(ssu_strcmp(argv[i], "-is") == 0){
			if(sed_opt.is_i == 1 || sed_opt.is_is == 1){
				sedUsage();
				return;
			}
			sed_opt.is_is = 1;
			if(i == argc-mnum-1){
				sedUsage();
				return;
			}
			sed_opt.istring = argv[++i];
		}
		else{
			sedUsage();
			return;
		}
	}

	if(sed_opt.is_s == 1){
		len = ssu_strlen(sed_opt.src_str);
		for(i=0; i<len; i++){
			if(isupper(*(sed_opt.src_str+i))){
				*(sed_opt.src_str+i) += 32;
			}
		}
	}

	if(sed_opt.is_s == 1){
		len = ssu_strlen(sed_opt.src_str);
		for(i=0; i<len; i++){
			if(isupper(*(sed_opt.src_str+i))){
				*(sed_opt.src_str+i) += 32;
			}
		}
	}

	if(sed_opt.is_es == 1){
		len = ssu_strlen(sed_opt.estring);
		for(i=0; i<len; i++){
			if(isupper(*(sed_opt.estring+i))){
				*(sed_opt.estring+i) += 32;
			}
		}
	}

	if(sed_opt.is_is == 1){
		len = ssu_strlen(sed_opt.istring);
		for(i=0; i<len; i++){
			if(isupper(*(sed_opt.istring+i))){
				*(sed_opt.istring+i) += 32;
			}
		}
	}

	// arguments print
	for(i=0; i<argc-mnum; i++){
		len = ssu_strlen(argv[i]);
		n = i+'0';
		write(STD_OUT, "arg", 3);
		write(STD_OUT, &n, 1);
		write(STD_OUT, " : ", 3);
		write(STD_OUT, argv[i], len);
		write(STD_OUT, "\n", 1);
	}

	// get target fullpath
	if(realpath(sed_opt.target, fullpath) == NULL){
		//err
		write(STD_OUT, "can't find target\n", 18);
		return;
	}

	searchFile(fullpath, &sed_opt, sed_opt.pathname, 0);

	if(ism == 1)
		free(sed_opt.pathname);

	return;
}

// 파일을 읽어서, 문자열을 치환하는 함수.
void replace(char *path, struct sedOption *sed_opt, char *pathname, int depth){
	char buf[BUFSIZE];
	char tmpfileName[] = "./tmpXXXXXX";
	char *dest;
	char *src;
	char printline;
	int src_len;
	int dest_len;
	int path_len;
	int linenum = 1;
	int readlen=0;
	int fd, fdtmp;
	int i, j, k;
	int tmp, sig;
	int tmp1, tmp2;
	int len;
	int isChanged = -1;
	int more=0;

	dest = sed_opt->dest_str;
	src = sed_opt->src_str;
	dest_len = ssu_strlen(dest);
	src_len = ssu_strlen(src);
	len = ssu_strlen(path);

	if((fd = open(path, O_RDONLY)) < 0){
		write(STD_OUT, path, len);
		write(STD_OUT, " : failed\n", 10);
		return;
	}

	// option check -e, -i

	if((fdtmp = mkstemp(tmpfileName)) < 0){
		write(STD_OUT, path, len);
		write(STD_OUT, " : failed\n", 10);
		return;
	}

	// read
	while(1){
		if((readlen = read(fd, buf, BUFSIZE)) < 0){
			write(STD_OUT, path, len);
			write(STD_OUT, " : failed\n", 10);
			close(fd);
			close(fdtmp);
			remove(tmpfileName);
			return;
		}

		if(readlen == 0){
			ssu_memset(buf, '\0', BUFSIZE);
			break;
		}

		tmp = (src_len-1) * -1;
		if(readlen == BUFSIZE){
			more = 1;
			if(lseek(fd, tmp, SEEK_CUR) < 0){
				write(STD_OUT, path, len);
				write(STD_OUT, " : failed\n", 10);
				remove(tmpfileName);
				return;
			}
		}
		else
			more = 0;

		for(i=0; i<readlen; i++){
			if(buf[i] == '\n'){
				linenum++;
			}

			if(more == 1 && i >= readlen-src_len+1)
				break;

			if(buf[i] == *src ||
				(sed_opt->is_s == 1 && (*src-buf[i]) == 32)){
				sig = 0;
				for(j=1, k=1; k<src_len; j++, k++){
					if(sed_opt->is_b == 1 && buf[i+j] == ' ' && *(src+k) != ' '){
						k--;
						continue;
					}
					if(buf[i+j] != *(src+k)){
						if((sed_opt->is_s == 1 && (*(src+k)-buf[i+j]) == 32));
						else{
							sig = -1;
							break;
						}
					}
				}

				if(sig == 0){
					if(sed_opt->is_p == 1){
						write(STD_OUT, path, len);
						write(STD_OUT, " : ", 3);
						tmp1 = 1;
						tmp2 = linenum;
						while(1){
							if(tmp2 / 10 == 0)
								break;
							tmp2/=10;
							tmp1*=10;
						}
						tmp2=linenum;
						while(1){
							printline = (tmp2/tmp1) + '0';
							write(STD_OUT, &printline, 1);
							tmp2%=tmp1;
							tmp1/=10;
							if(tmp1 < 10){
								printline = (tmp2%10) + '0';
								write(STD_OUT, &printline, 1);
								break;
							}
						
						}
						write(STD_OUT, "\n", 1);
					}
					write(fdtmp, dest, dest_len);
					i+= (j-1);
					isChanged = 1;
					continue;
				}
			}
			
			if(write(fdtmp, &buf[i], 1) < 0){
				write(STD_OUT, path, len);
				write(STD_OUT, " : failed\n", 10);
				close(fd);
				close(fdtmp);
				if(remove(tmpfileName) < 0) return;
				return;
			}
		}
		ssu_memset(buf, '\0', BUFSIZE);
	}

	//check is changed
	if(isChanged > 0){
		if(lseek(fdtmp, 0, SEEK_SET) < 0){
			write(STD_OUT, path, len);
			write(STD_OUT, " : failed\n", 10);
			remove(tmpfileName);
			return;
		}

		close(fd);

		if(sed_opt->is_P == 1){
			path = pathname;
		}
		
		if((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644)) < 0){
				write(STD_OUT, path, len);
				write(STD_OUT, " : failed\n", 10);
				close(fdtmp);
				if(remove(tmpfileName) < 0) return;
				return;
		}

		while(1){
			if((readlen = read(fdtmp, buf, BUFSIZE)) < 0){
				write(STD_OUT, path, len);
				write(STD_OUT, " : failed\n", 10);
				close(fd);
				close(fdtmp);
				if(remove(tmpfileName) < 0) return;
				return;
			}

			if(readlen == 0){
				close(fd);
				close(fdtmp);
				remove(tmpfileName);
				break;
			}

			if(write(fd, buf, readlen) < 0){
				write(STD_OUT, path, len);
				write(STD_OUT, " : failed\n", 10);
				close(fd);
				close(fdtmp);
				remove(tmpfileName);
				return;
			}

			ssu_memset(buf, '\0', BUFSIZE);
		}

		write(STD_OUT, path, len);
		write(STD_OUT, " : success\n", 11);
	}
	else{
		write(STD_OUT, path, len);
		write(STD_OUT, " : failed\n", 10);
	}
	//lseek

	close(fd);
	close(fdtmp);
	if(sed_opt->is_P != 1 || (sed_opt->is_P == 1 && isChanged<0))
		remove(tmpfileName);
	return;
}

// 현재 파일이 디렉토리인지 레귤러파일인지 확인하는 함수.
void searchFile(char *path, struct sedOption *sed_opt, char *pathname, int depth){
	struct stat statbuf;
	char *tmp = NULL;
	int check;
	int len;

	if(sed_opt->is_d == 1 && depth > sed_opt->depth){
		return;
	}

	if(stat(path, &statbuf) < 0){
		return;
	}
	if(S_ISDIR(statbuf.st_mode) != 0){
		if(sed_opt->is_P == 1){
			//mkdir
			mkdir(pathname, 0755);
		}
		searchDir(path, sed_opt, pathname, depth+1);
	}
	else if(S_ISREG(statbuf.st_mode) != 0){
		len = ssu_strlen(path);

		//-u option check, username
		if(sed_opt->is_u == 1 && statbuf.st_uid != sed_opt->uid){
			write(STD_OUT, path, len);
			write(STD_OUT, " : failed\n", 10);
			return;
		}

		if(sed_opt->is_i == 1 || sed_opt->is_is == 1){
			check = checkString(path, sed_opt, sed_opt->istring);

			if(sed_opt->is_i == 1 && check != 1){
				write(STD_OUT, path, len);
				write(STD_OUT, " : failed\n", 10);
				return;
			}
			else if(sed_opt->is_is == 1 && check != 1){
				write(STD_OUT, path, len);
				write(STD_OUT, " : failed\n", 10);
				return;
			}
		}

		if(sed_opt->is_e == 1 || sed_opt->is_es == 1){
			check = checkString(path, sed_opt, sed_opt->estring);
			if(sed_opt->is_e == 1 && check != 0){
				write(STD_OUT, path, len);
				write(STD_OUT, " : failed\n", 10);
				return;
			}
			else if(sed_opt->is_es == 1 && check != 0){
				write(STD_OUT, path, len);
				write(STD_OUT, " : failed\n", 10);
				return;
			}
		}
		replace(path, sed_opt, pathname, depth+1);
	}
	return;
}

// 디렉토리 안을 순회하는 함수.
void searchDir(char *path, struct sedOption *sed_opt, char *pathname, int depth){
	struct dirent *dirp;
	DIR *dp;
	char *nPath = NULL;
	char *pPath = NULL;
	int p, d;

	if(sed_opt->is_d == 1 && depth > sed_opt->depth)
		return;

	if((dp = opendir(path)) == NULL)
	{
		//
		return;
	}

	while((dirp = readdir(dp)) != NULL){
		if(ssu_strcmp(dirp->d_name, ".") == 0){
			continue;
		}
		if(ssu_strcmp(dirp->d_name, "..") == 0){
			continue;
		}
		p = ssu_strlen(path);
		d = ssu_strlen(dirp->d_name);

		nPath = (char *)malloc(p+d+2);
		ssu_memset(nPath, '\0', p+d+2);
		ssu_strcpy(nPath, path);
		if(path[p-1] != '/'){
			nPath[p] = '/';
			ssu_strcpy(&nPath[p+1], dirp->d_name);
			nPath[p+d+1] = '\0';
		}
		else{
			ssu_strcpy(&nPath[p], dirp->d_name);
			nPath[p+d] = '\0';
		}

		if(sed_opt->is_P == 1){
			p = ssu_strlen(pathname);
			pPath = (char *)malloc(p+d+2);
			ssu_memset(pPath, '\0', p+d+2);
			ssu_strcpy(pPath, pathname);
			if(path[p-1] != '/'){
				pPath[p] = '/';
				ssu_strcpy(&pPath[p+1], dirp->d_name);
				pPath[p+d+1] = '\0';
			}
			else{
				ssu_strcpy(&pPath[p], dirp->d_name);
				pPath[p+d] = '\0';
			}
		}
		searchFile(nPath, sed_opt, pPath, depth);
		free(nPath);
	}
	closedir(dp);
}

// 파일에 string이 있는지 확인하는 함수.
int checkString(char *path, struct sedOption *sed_opt, char *str){
	int fd;
	fd = open(path, O_RDONLY);
	char *string = str;
	int stringlen = ssu_strlen(string);
	char buf[BUFSIZE];
	int sig;
	int tmp;
	int readlen;
	int i, j, k;
	int nocheck = 0;
	int more=0;

	ssu_memset(buf, '\0', BUFSIZE);

	if(sed_opt->is_is == 1 || sed_opt->is_es == 1)
		nocheck = 1;

	while(1){
		if((readlen = read(fd, buf, BUFSIZE)) < 0){
			close(fd);
			return -1;
		}

		if(readlen == 0){
			ssu_memset(buf, '\0', BUFSIZE);
			break;
		}

		tmp = stringlen * -1;
		if(readlen == BUFSIZE){
			more = 1;
			if(lseek(fd, tmp, SEEK_CUR) < 0){
				return -1;
			}
		}
		else
			more = 0;

		for(i=0; i<readlen; i++){
			if(more == 1 && i >= readlen-stringlen+1)
				break;

			if(buf[i] == *string ||
				(nocheck == 1 && (*string-buf[i]) == 32)){
				sig = 0;
				for(j=1; j<stringlen; j++){
					if(buf[i+j] != *(string+j)){
						if((nocheck == 1 && (*(string+j)-buf[i+j]) == 32));
						else{
							sig = -1;
							break;
						}
					}
				}

				if(sig == 0){
					i+= (j-1);
					return 1;
				}
			}
		}
		ssu_memset(buf, '\0', BUFSIZE);
	}

	close(fd);
	return 0;
}