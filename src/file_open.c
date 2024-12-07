#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "file_open.h"

#define PROC_DIR "/proc"
#define LOCKS_FILE "/proc/locks"

void find_pids_with_file(const char *filename, int *list_pid) {
    DIR *dir;
    struct dirent *entry;
    char fd_path[1024];
    char target_file[1024];
    char symlink_target[1024];
    ssize_t len;
    dir = opendir(PROC_DIR);
    if (dir == NULL) {
        perror("Failed to open /proc directory");
        exit(1);
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR || !isdigit(entry->d_name[0])) {
            continue;
        }

        int pid_check = atoi(entry->d_name);

        int cnt=1;
        for(int i=0; i<sz_delep; i++){
            if(pid_check == list_pid[i])
                cnt = 0;
        }
        if(cnt == 0) 
            continue;

        snprintf(fd_path, sizeof(fd_path), "%s/%s/fd", PROC_DIR, entry->d_name);

        DIR *fd_dir = opendir(fd_path);
        if (fd_dir == NULL) {
            continue;  
        }
        struct dirent *fd_entry;
        while ((fd_entry = readdir(fd_dir)) != NULL) {
            if (strcmp(fd_entry->d_name, ".") == 0 || strcmp(fd_entry->d_name, "..") == 0) {
                continue;
            }
            snprintf(target_file, sizeof(target_file), "%s/%s/fd/%s", PROC_DIR, entry->d_name, fd_entry->d_name);
            len = readlink(target_file, symlink_target, sizeof(symlink_target) - 1);
            if (len != -1) {
                symlink_target[len] = '\0';  
                if (strcmp(symlink_target, filename) == 0) {
                    printf("PID: %s has the file %s open\n", entry->d_name, filename);
                    pid_t pid = atoi(entry->d_name);
                    list_pid[sz_delep] = pid;
                    sz_delep++;
                    list_pid = (pid_t *)realloc(list_pid, (sz_delep+1) * sizeof(pid_t));
                }
            }
        }
        closedir(fd_dir);
    }
    closedir(dir);
}
