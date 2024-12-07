#ifndef FILE_OPEN_H
#define FILE_OPEN_H

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

void find_pids_with_file(const char *filename, int *list_pid);


#endif // FILE_OPEN_H