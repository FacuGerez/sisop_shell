#ifndef HANDLER_H
#define HANDLER_H

void init_sigchild_handler(void);

void disable_sigchild_handler(void);

void free_signalstack(void);

#endif
