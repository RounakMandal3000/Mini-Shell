#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "exec_comm.h"

int execute_command(char **args, int mode){
    pid_t pid;
    int status;
    pid = fork();
    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("error in new_process: child process");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
        perror("error in new_process: forking");
    else{
        if(mode==0){
            do{
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        else{
            printf("Background job started with PID: %d\n", pid);
            
        }
    }
    return -1;
}

int handle_redirection(char **args) {
    int fd_out = -1, fd_in = -1;
    int saved_stdout, saved_stdin;

    // Save the original stdout and stdin
    saved_stdout = dup(STDOUT_FILENO);
    saved_stdin = dup(STDIN_FILENO);
    int l=0;
    for (int i = 0; args[i] != NULL; i++) {
        l++;
        // Output redirection ">"
        if (strcmp(args[i], ">") == 0) {
            fd_out = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd_out == -1) {
                perror("myshell");
                return -1;
            }
            // Redirect stdout to the file
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);  
            args[i] = NULL; 
        }
        // Input redirection "<"
        else if (strcmp(args[i], "<") == 0) {
            fd_in = open(args[i + 1], O_RDONLY);
            if (fd_in == -1) {
                perror("myshell");
                return -1;
            }
            // Redirect stdin from the file
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);  
            args[i] = NULL; 
            // printf("%d \n", fd_in);
        }
    }
    int mode=0;
    if(strcmp(args[l-1], "&") == 0){
        mode = 1;
        args[l-1] = NULL;
    }
    
    int status = execute_command(args, mode);
    // // If redirection was done, continue writing to the standard output again
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout); 
    }
    if (saved_stdin != -1) {
        // Restore the original stdin
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin); 
    }
    return status;
}

int is_pipeline(char **args){
    for(int i=0; args[i]!=NULL; i++){
        if(strcmp(args[i], "|") == 0){
            return 1;
        }
    }
    return 0;    
}
void pipeline_command(char **args){
    char **commands[10];
    int num_pipes = 0;
    int cmd_index = 0;
    int arg_index = 0;
    for (int i = 0; i < 10; i++) {
        commands[i] = malloc(100 * sizeof(char *)); 
    }
    for(int i=0; args[i]!=NULL; i++){
        if(strcmp(args[i], "|") == 0){
            commands[cmd_index][arg_index] = NULL; 
            cmd_index++; 
            arg_index = 0;
            num_pipes++;
        }
        else{
            commands[cmd_index][arg_index++] = args[i];
        }
    }

    commands[cmd_index][arg_index] = NULL;
    // for (int i = 0; i <= cmd_index; i++) {
    //     printf("Command %d: ", i + 1);
    //     for (int j = 0; commands[i][j] != NULL; j++) {
    //         printf("%s ", commands[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    
    num_pipes++;
    int pipefds[num_pipes][2];
    for(int i=0; i<num_pipes; i++){
        if(pipe(pipefds[i])<0){
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid[num_pipes];
    for(int i=0; i<num_pipes; i++){
        int status;
        pid[i] = fork();
        if (pid[i] == 0){
            if(i>0)
                dup2(pipefds[i-1][0], STDIN_FILENO);  
            if((i+1)!=num_pipes)
                dup2(pipefds[i][1], STDOUT_FILENO); 

            for (int j = 0; j < num_pipes; j++){
                close(pipefds[j][0]);
                close(pipefds[j][1]);
            }
            if(execvp(commands[i][0], commands[i])==-1){
                perror("error in new_process: child process");
            }
            exit(EXIT_FAILURE);
        }
    }   
    for (int i = 0; i < num_pipes; i++){
        close(pipefds[i][0]);
        close(pipefds[i][1]);
    }

    for (int i = 0; i < num_pipes; i++) {
        waitpid(pid[i], NULL, 0);
    }

    for (int i = 0; i < 10; i++) {
        free(commands[i]);
    }
}

int has_wildcard(char **args){
    for(int i=0; args[i]!=NULL; i++){
        if(strchr(args[i], '*') || strchr(args[i], '?')){
            return 1;
        }
    }
    return 0;
}

void change_wildcard(char ***args){
    char **temp = malloc(1000 * sizeof(char *));
    int count=0;
    for(int i=0; (*args)[i]!=NULL; i++){
        if(strchr((*args)[i], '*') || strchr((*args)[i], '?')){
            glob_t results;
            int ret = glob((*args)[i], GLOB_ERR, NULL, &results);
            if (ret != 0) {
                printf("No files matched the pattern or an error occurred.\n");
                continue;
            }
            for (size_t j = 0; j < results.gl_pathc; j++) {
                temp[count] = malloc((strlen(results.gl_pathv[j]) + 1) * sizeof(char)); 
                strcpy(temp[count], results.gl_pathv[j]); 
                count++;
            }
            globfree(&results);
        }
        else{
            temp[count] = malloc((strlen((*args)[i]) + 1) * sizeof(char));
            strcpy(temp[count], (*args)[i]); 
            count++;
        }
    }
    temp[count] = NULL;
    *(args) = temp;
}

void sigint_handler(int signum) {
    printf("\nPress Ctrl+D to exit the shell, or continue using it.\n");
    // display_prompt();
    fflush(stdout);
}
