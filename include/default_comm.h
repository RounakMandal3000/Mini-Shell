#ifndef DEF_COMM_H
#define DEF_COMM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <glob.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <sys/stat.h>

int is_builtin_command(char **args);   
void execute_builtin_command(char **args); 

#endif // DEF_COMM_H