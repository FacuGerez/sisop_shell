#include <unistd.h>

#include "printstatus.h"

// prints information of process' status
void
print_status_info(struct cmd *cmd)
{
	const char *action;

	if (strlen(cmd->scmd) == 0 || cmd->type == PIPE)
		return;

	if (WIFEXITED(status)) {
		action = "exited";
		status = WEXITSTATUS(status);
	} else if (WIFSIGNALED(status)) {
		action = "killed";
		status = -WTERMSIG(status);
	} else if (WTERMSIG(status)) {
		action = "stopped";
		status = -WSTOPSIG(status);
	} else {
		return;
	}

#ifndef SHELL_NO_INTERACTIVE
	if (isatty(1)) {
		fprintf(stdout,
		        "%s	Program: [%s] %s, status: %d %s\n",
		        COLOR_BLUE,
		        cmd->scmd,
		        action,
		        status,
		        COLOR_RESET);
	}
#endif
}

// prints info when a background process is spawned
void
print_back_info(struct cmd *back)
{
#ifndef SHELL_NO_INTERACTIVE
	if (isatty(1)) {
		fprintf(stdout, "%s  [PID=%d] %s\n", COLOR_BLUE, back->pid, COLOR_RESET);
	}
#endif
}

// Converts an (unsigned) integer to string.
static void
convert_uint_to_str(char *buf, unsigned int num)
{
	int i = 0;

	// Parse digits.
	if (num == 0) {
		buf[i++] = '0';
	} else {
		while (num > 0) {
			// ASCII 48 = '0'.
			buf[i++] = (char) (48 + num % 10);
			num /= 10;
		}
	}

	// Reverse digits.
	for (int j = 0; j < i / 2; j++) {
		const char tmp = buf[j];
		buf[j] = buf[i - j - 1];
		buf[i - j - 1] = tmp;
	}

	buf[i] = '\0';
}

// Concatenates two strings.
static void
concatenate_str(char *dest, const char *src)
{
	// Instead of reading strlen(src) bytes (can
	// produce a buffer overflow), this reads up
	// to the remaining space of the dest buffer.
	strncat(dest, src, sizeof(dest) - strlen(dest) - 1);
}

// Prints information of a background process' status.
void
print_back_status_info(pid_t pid)
{
	const char *action;

	// TODO: Consider a function to avoid duplicate code.
	if (WIFEXITED(status)) {
		action = "exited";
		status = WEXITSTATUS(status);
	} else if (WIFSIGNALED(status)) {
		action = "killed";
		status = -WTERMSIG(status);
	} else if (WTERMSIG(status)) {
		action = "stopped";
		status = -WSTOPSIG(status);
	} else {
		return;
	}

#ifndef SHELL_NO_INTERACTIVE
	// This is called inside a signal handler, so it must
	// be async-signal-safe code. This means that printf
	// and similar functions CANNOT be used (as they use
	// malloc(3), which is NOT async-signal-safe).
	// That's why manual formatting and writing is needed
	// here, using strncat(3) and write(2), which are both
	// required to be async-signal-safe in POSIX.
	char message[BUFLEN] = COLOR_BLUE "Background program: [PID=";

	char pid_str[32];
	convert_uint_to_str(pid_str, pid);
	concatenate_str(message, pid_str);

	concatenate_str(message, "] ");

	concatenate_str(message, action);

	concatenate_str(message, ", status: ");

	char status_str[32];
	convert_uint_to_str(status_str, status);
	concatenate_str(message, status_str);

	concatenate_str(message, COLOR_RESET " ");

	if (write(STDOUT_FILENO, message, strlen(message)) == -1) {
		exit(EXIT_FAILURE);
	}
#endif
}
