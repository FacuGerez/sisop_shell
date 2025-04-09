#include "utils_exec.h"
#include "types.h"
#include "freecmd.h"

// Runs the command.
void
run_exec_cmd(struct execcmd *cmds)
{
	// TODO: I'm not searching in path the executable, I dont know how to do it.
	execvp(cmds->argv[0], cmds->argv);
	// if I return from the execvp call, it means I have an error.
	free_command((struct cmd *) cmds);
	exit(EXIT_FAILURE);
}

// opens the file in which the stdin/stdout/stderr
// flow will be redirected, and returns
// the file descriptor
// returns -1 if opening fails
int
open_redir_fd(char *file, int flags)
{
	return open(file, flags, S_IWUSR | S_IRUSR);
}

// Redirects a file descriptor to a file.
// The user may specify the flags to use when
// opening the file.
// If the file indicates another file descriptor
// (using '&'), then redirects to that other file
// descriptor.
// IMPORTANT: In the last case, the file descriptor
// IS NOT CLOSED (to avoid cases where, for example,
// stderr is redirected to stdout, but stdout is
// still needed for the program to work).
// The user may close it if needed.
// Returns the file descriptor on success,
// or -1 if the opening fails.
int
redirect_fd(int fd, char *filename, int flags)
{
	int file_fd;
	if (filename[0] == '&') {
		file_fd = strtol(&filename[1], NULL, 10);
		dup2(file_fd, fd);
	} else {
		file_fd = open_redir_fd(filename, O_CLOEXEC | flags);
		if (file_fd != -1) {
			dup2(file_fd, fd);
			close(file_fd);
		}
	}
	return file_fd;
}
