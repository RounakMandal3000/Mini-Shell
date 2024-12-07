#ifndef FILE_LOCK_H
#define FILE_LOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <sys/stat.h>

extern int sz_delep;

void pid_file_finder(int pid, const char *filename, int *list_pid);
void find_pids_with_file_lock(const char *file_path, int *list_pid);

#endif // FILE_LOCK_H