#ifndef UTILS_EXEC_H
#define UTILS_EXEC_H

#include "types.h"

void run_exec_cmd(struct execcmd *cmds);

int open_redir_fd(char *file, int flags);

int redirect_fd(int fd, char *filename, int flags);

#endif
