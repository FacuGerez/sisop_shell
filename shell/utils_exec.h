#ifndef UTILS_EXEC_H
#define UTILS_EXEC_H

#include "types.h"

void run_exec_cmd(struct execcmd *cmd);

int open_redir_fd(const char *file, int flags);

int redirect_fd(int fd, const char *file, int flags);

#endif
