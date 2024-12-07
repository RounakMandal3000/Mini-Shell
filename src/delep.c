#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "delep.h"
#include "file_lock.h"
#include "file_open.h"

#define PROC_DIR "/proc"
#define LOCKS_FILE "/proc/locks"
int sz_delep = 0;

int is_delep(char **args){
    for(int i=0; args[i]!=NULL; i++){
        if(strcmp("delep", args[i])==0){
            return 1;
        }
    }
    return 0;
}

int execute_delep(char **args){
    // printf("%d\n", 1);
    pid_t pid;
    int status;
    pid = fork();
    
    if(pid==0){
        sz_delep = 0;
        if(args[1]==NULL){
            perror("Wrong format of command");
            exit(EXIT_FAILURE);
        }
        const char *file_path = malloc(strlen(args[1]) + 1); // +1 for the null terminator
        if (file_path == NULL) {
            perror("malloc");
            exit(1);
        }
        strcpy((char *)file_path, args[1]);
        int *list_pid = (int *)malloc(1 * sizeof(int));
        find_pids_with_file_lock(file_path, list_pid);
        find_pids_with_file(file_path, list_pid);
        
        int *unique_list_pid = (int *)malloc(1 * sizeof(int));
        int sz_un_delep = 0;
        for(int i=0; i<sz_delep; i++){
            int cnt=1;
            for(int j=0; j<i; j++){
                if(i!=j && list_pid[i]==list_pid[j]){
                    cnt = 0;
                }
            }
            if(cnt==1){
                unique_list_pid[sz_un_delep] = list_pid[i];
                sz_un_delep++;
                unique_list_pid = (int *)realloc(unique_list_pid, (sz_un_delep+1) * sizeof(int));

            }
        }

        for(int i=0; i<sz_un_delep; i++){
            printf("%d\n", unique_list_pid[i]);
        }
        char* buf;
        if(sz_un_delep>=1){
            buf = readline("Do you want to kill these processes :(? Press Y to confirm, anything else to not: ");
            if(strcmp(buf, "y")==0 || strcmp(buf, "Y")==0){
                for(int i=0; i<sz_un_delep; i++)
                    kill_process(unique_list_pid[i]);
            }
        }
        free(list_pid);
        free(unique_list_pid);
        exit(EXIT_FAILURE);
    }
    else if(pid<0){
        perror("Forking issue in delep!");
    }
    else{ // parent process
        do{
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return -1;
}

void kill_process(int pid) {
    if (kill(pid, SIGKILL) == 0) {
        printf("Successfully killed process with PID: %d\n", pid);
    } else {
        perror("kill");
    }
}

