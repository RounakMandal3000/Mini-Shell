#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "delep.h"
#include "file_lock.h"
#include "file_open.h"
#include "default_comm.h"
#include "exec_comm.h"
#include "shell_input.h"

volatile sig_atomic_t shell_running = 1;

int main(){
    char *input;
    char **args;
    int status = 1;
    init_hist();
    using_history();
    stifle_history(MAX_HISTORY);
    signal(SIGINT, sigint_handler);
    while(shell_running){
        while(status){
            display_prompt();
            input = read_input();
            args = parse_input(input);
            if(args[0]!=NULL){
                if(is_builtin_command(args)){
                    execute_builtin_command(args);
                }
                else if(is_delep(args)){
                    status = execute_delep(args);
                }
                else{
                    int tf = has_wildcard(args);
                    if(tf == 1){
                        change_wildcard(&args);
                    }
                    if(is_pipeline(args)==1){
                        pipeline_command(args);
                    }
                    else
                        status = handle_redirection(args);
                }
            }
            free(input);
            free(args);
        }
    }
    return 0;
}