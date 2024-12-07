#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "default_comm.h"

int is_builtin_command(char **args) {
    return (strcmp(args[0], "exit") == 0 || strcmp(args[0], "cd") == 0 || strcmp(args[0], "pwd") == 0);
}

void execute_builtin_command(char **args){
    if(strcmp(args[0], "exit") == 0){
        exit(0);
    }
    else if(strcmp(args[0], "cd") == 0 ){ // handle spaces here
        fprintf(stdout, args[1], "\n");
        if (args[1] == NULL) 
            fprintf(stderr, "myshell: expected argument to \"cd\"\n");
        else if (chdir(args[1]) != 0)
            perror("myshell");
        else{
            fprintf(stdout, "Directory changed successfully.\n");
        }
    }
    else if(strcmp(args[0], "pwd") == 0 ){
        char cwd[1024]; 
        if (getcwd(cwd, sizeof(cwd)) != NULL) 
            printf("Current working directory: %s\n", cwd);
        else 
            perror("getcwd() error");  
    }
}
