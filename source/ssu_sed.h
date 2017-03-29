#ifndef SSU_SED_H
#define SSU_SED_H

struct sedOption {
    int is_b;
    int is_u;
    int is_i;
    int is_e;
    int is_d;
    int is_p;
    int is_s;
    int is_P;
    int is_es;
    int is_is;
    int depth;
    int uid;
    char *target;
    char *src_str;
    char *dest_str;
    char *option;
    char *estring;
    char *istring;
    char *pathname;
};

void searchDir(char *path, struct sedOption *sed_opt, int depth);
void searchFile(char *path, struct sedOption *sed_opt, int depth);
void replace(char *path, struct sedOption *sed_opt, int depth);
int checkString(char *path, struct sedOption *sed_opt, char* str);

#endif
