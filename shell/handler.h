#ifndef HANDLER_H
#define HANDLER_H

#include "defs.h"
#include "types.h"

stack_t init_sigchild_handler(void);

void free_signalstack(stack_t ss);

#endif
