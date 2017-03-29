#ifndef SSU_STRING_H
#define SSU_STRING_H

int ssu_strcmp(char *, char *);
size_t ssu_strlen(char *str);
void *ssu_memset(void *s, int c, size_t n);
char *ssu_strcpy(char *dest, char *src);
char *ssu_strncpy(char *dest, char *src, int n);
char *ssu_strstr(char *, char *);
int ssu_memcmp(void *m1, void *m2, size_t count);
#endif
