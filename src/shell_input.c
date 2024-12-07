#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "shell_input.h"

char** command_history;
unsigned history_count = 0;
const int MAX_HISTORY = 1000;

void display_prompt();                  
char *read_input();                     
char **parse_input(char *input); 
void init_hist();
void add_command_to_history(const char *command);

void display_prompt(){
    fflush(stdout);    
}

char *read_input(){
    static int current_index = 0;
    char* buf;
    buf = readline("myshell>> ");
    if (buf && strlen(buf) != 0) {
        add_command_to_history(buf);
        add_history(buf);
        current_index = history_count;
    }
    return buf;
}

char **parse_input(char *input){
    int buff_size = 64;
    char **tokens = malloc(buff_size * sizeof(char *));
    if(!tokens){
        fprintf(stderr, "No memory available for storing command tokens");
        exit(EXIT_FAILURE);
    }
    char *token;
    int position = 0;
    token = strtok(input, " ");
    while(token!=NULL){
        tokens[position++] = token;
        if(position >= buff_size){
            buff_size += 64;
            tokens = realloc(tokens, buff_size * sizeof(char *));
            if(!tokens){
                fprintf(stderr, "No memory available for storing command tokens");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " ");
    }
    tokens[position] = NULL; // Null-terminate the array
    return tokens;
}

void init_hist(){
    command_history = malloc(MAX_HISTORY * sizeof(char*));
    if (command_history == NULL) {
        perror("Failed to allocate memory for history");
        exit(EXIT_FAILURE);
    }
}

void add_command_to_history( const char *command )
{
   if (history_count < MAX_HISTORY) {
        command_history[history_count++] = strdup( command );
   } else {
        free( command_history[0] );
        for (unsigned index = 1; index < MAX_HISTORY; index++) {
            command_history[index - 1] = command_history[index];
        }
        command_history[MAX_HISTORY - 1] = strdup( command );
    }
}