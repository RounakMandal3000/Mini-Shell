#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>


// Constants
extern const int MAX_HISTORY;

// Global Variables
extern char **command_history;
extern unsigned history_count;

// Function Prototypes
void display_prompt();
char *read_input();
char **parse_input(char *input);
void init_hist();
void add_command_to_history(const char *command);

#endif // SHELL_H
