#ifndef EXEC_H
#define EXEC_H

#include "types.h"

extern struct cmd *parsed_pipe;

void exec_cmd(struct cmd *cmd);

#endif  // EXEC_H
