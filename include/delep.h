#ifndef DELEP_H
#define DELEP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <sys/stat.h>


extern int sz_delep;

int is_delep(char **args);
int execute_delep(char **args);
void kill_process(int pid);

#endif // DELEP_H