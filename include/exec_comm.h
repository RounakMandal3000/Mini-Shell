#ifndef EXEC_H
#define EXEC_H

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


int execute_command(char **args, int mode);
void pipeline_command(char **args);
int is_pipeline(char **args);
int handle_redirection(char **args);  
int has_wildcard(char **args);
void change_wildcard(char ***args);
void sigint_handler(int signum);

#endif // EXEC_H