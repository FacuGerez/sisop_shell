#include "handler.h"

#include "printstatus.h"

// Signal handler to be executed on SIGCHLD.
static void
sigchild_handler(int signum)
{
	pid_t pid;
	int status;

	// The while loop waits for all processes that might
	// be "merged" into only one signal, if one finishes
	// immediately after another.
	while ((pid = waitpid(0, &status, WNOHANG)) > 0) {
		print_back_status_info(pid, status);
	}
}

// Initializes the sigchild handler on an alternative
// stack.
stack_t
init_sigchild_handler()
{
	// Initialize alternative stack for signal handler.
	stack_t ss;

	ss.ss_sp = malloc(SIGSTKSZ);
	if (ss.ss_sp == NULL) {
		exit(EXIT_FAILURE);
	}

	ss.ss_size = SIGSTKSZ;
	ss.ss_flags = 0;
	if (sigaltstack(&ss, NULL) == -1) {
		exit(EXIT_FAILURE);
	}

	// Initialize signal handler.
	struct sigaction sa;

	// SA_ONSTACK indicates that the handler will use the
	// alternative stack.
	// SA_RESTART allows syscalls that might be interrupted
	// by the handler to restart.
	sa.sa_flags = SA_ONSTACK | SA_RESTART;
	sa.sa_handler = sigchild_handler;
	// Do not block other signals.
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		exit(EXIT_FAILURE);
	}

	return ss;
}

// Frees the alternative stack.
void
free_signalstack(stack_t ss)
{
	if (ss.ss_sp != NULL) {
		free(ss.ss_sp);
		ss.ss_sp = NULL;
	}
}
