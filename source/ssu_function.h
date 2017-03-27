#ifndef SSU_FUNCTION_H
#define SSU_FUNCTION_H

void ssu_shell();
void ssu_sed(int argc, char *argv[]);

struct ssu_function{
	char *cmd;
	void (*func)(int argc, char *argv[]);
};

#endif
