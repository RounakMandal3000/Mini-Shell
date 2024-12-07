#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "file_lock.h"

#define PROC_DIR "/proc"
#define LOCKS_FILE "/proc/locks"

void pid_file_finder(int pid, const char *filename, int *list_pid){
    DIR *dir;
    struct dirent *entry;
    char fd_path[1024];
    char target_file[1024];
    char symlink_target[1024];
    ssize_t len;
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Failed to open /proc directory");
        exit(1);
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR || !isdigit(entry->d_name[0])) {
            continue;
        }
        pid_t pid1 = atoi(entry->d_name);
        if(pid1!=pid){
            continue;
        }
        snprintf(fd_path, sizeof(fd_path), "%s/%s/fd", "/proc", entry->d_name);
        DIR *fd_dir = opendir(fd_path);
        if (fd_dir == NULL) {
            continue;  
        }
        struct dirent *fd_entry;
        while ((fd_entry = readdir(fd_dir)) != NULL) {
            if (strcmp(fd_entry->d_name, ".") == 0 || strcmp(fd_entry->d_name, "..") == 0) {
                continue;
            }
            snprintf(target_file, sizeof(target_file), "%s/%s/fd/%s", "/proc", entry->d_name, fd_entry->d_name);
            len = readlink(target_file, symlink_target, sizeof(symlink_target) - 1);
            if (len != -1) {
                symlink_target[len] = '\0';  
                if (strcmp(symlink_target, filename) == 0) {
                    // printf("%s\n", symlink_target);
                    // printf("PID: %s has the file %s open\n", entry->d_name, filename);
                    printf("PID: %d has the file %s locked\n", pid, filename);
                    pid_t pid = atoi(entry->d_name);
                    // kill_process(pid);
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


void find_pids_with_file_lock(const char *file_path, int *list_pid) {
    FILE *locks = fopen(LOCKS_FILE, "r");
    if (locks == NULL) {
        perror("fopen");
        return;
    }
    // printf("Processes holding a lock on the file (%s):\n", file_path);
    char line[256];
    while (fgets(line, sizeof(line), locks)) {
        // printf("Line: %s\n", line); 

        char type[16];
        int pid;
        char lock_file[1024];  // Buffer to store the lock file path
        unsigned int major, minor;
        unsigned long long inode;

        // Parse the relevant parts of the line
        int x = sscanf(line, "%*d: %15s %*s %*s %d %x:%x:%llu %1023s", 
                        type, &pid, &major, &minor, &inode, lock_file);
        pid_file_finder(pid, file_path, list_pid);
    }
    fclose(locks);

}
