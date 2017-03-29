#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ssu_string.h"

int ssu_strcmp(char * str1, char *str2){
	char c1, c2;
	while(1){
		c1 = *str1++;
		c2 = *str2++;
		if(c1 != c2){
			if (c1 > c2)
				return 1;
			else
				return -1;
		}
		if(!c1)
			return 0;
	}
}

size_t ssu_strlen(char *str){
	char *tmp = str;
	int rst=0;
	while(*tmp != '\0'){
		rst++;
		tmp++;
	}
	return rst;
}

void *ssu_memset(void *s, int c, size_t n){
	char *tmp = s;
	while(n--){
		*tmp = c;
		tmp++;
	}
	return tmp;
}

char *ssu_strcpy(char *dest, char *src){
	char *tmp = dest;
	while(*src != '\0'){
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return tmp;
}

char *ssu_strncpy(char *dest, char *src, int n){
	int i=0;
	for(i = 0; i<n; i++){
		*(dest+i) = *(src+i);
	}
	return dest;
}

char *ssu_strstr(char *str1, char *str2){
	size_t len1, len2;
	len2 = ssu_strlen(str2);
	if(!len2)
		return (char *)str1;
	len1 = ssu_strlen(str1);
	while(len1 >= len2){
		len1--;
		if(!ssu_memcmp(str1, str2, len2))
			return (char *)str1;
		str1++;
	}
	return NULL;
}

int ssu_memcmp(void *m1, void *m2, size_t count){
	char *s1;
	char *s2;
	int i=0;
	int res = 0;

	for(s1 = m1, s2 = m2; i < count; i++){
		s1++; s2++;
		if((res = *s1-*s2) != 0)
			break;
	}
	return res;
}