#include "exec.h"

// sets "key" with the key part of "arg"
// and null-terminates it
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  key = "KEY"
//
static void
get_environ_key(char *arg, char *key)
{
	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets "value" with the value part of "arg"
// and null-terminates it
// "idx" should be the index in "arg" where "=" char
// resides
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  value = "value"
//
static void
get_environ_value(char *arg, char *value, int idx)
{
	size_t i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void
set_environ_vars(char **eargv, int eargc)
{
	// Your code here
}

// opens the file in which the stdin/stdout/stderr
// flow will be redirected, and returns
// the file descriptor
// returns -1 if opening fails
static int
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
static int
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

// executes a command - does not return
void
exec_cmd(struct cmd *cmd)
{
	// To be used in the different cases
	struct execcmd *e;
	struct backcmd *b;
	struct execcmd *r;
	struct pipecmd *p;

	switch (cmd->type) {
	case EXEC:
		// spawns a command
		e = (struct execcmd *) cmd;

		// execvpe(e->argv[FILENAME], e->argv, e->eargv);
		execvp(e->argv[FILENAME], e->argv);

		break;

	case BACK: {
		// runs a command in background
		//
		// Your code here
		printf("Background process are not yet implemented\n");
		_exit(-1);
		break;
	}

	case REDIR: {
		// changes the input/output/stderr flow
		r = (struct execcmd *) cmd;

		// This case is the only one that might fail if the
		// file does not exist.
		if (strlen(r->in_file) &&
		    redirect_fd(STDIN, r->in_file, O_RDONLY) == -1)
			break;

		// These are safe because if the file does not exist,
		// then a new file is created.
		if (strlen(r->out_file))
			redirect_fd(STDOUT, r->out_file, O_RDWR | O_CREAT | O_TRUNC);

		if (strlen(r->err_file))
			redirect_fd(STDERR, r->err_file, O_RDWR | O_CREAT | O_TRUNC);


		// execvpe(e->argv[FILENAME], e->argv, e->eargv);
		execvp(r->argv[FILENAME], r->argv);

		break;
	}

	case PIPE: {
		// pipes two commands
		//
		// Your code here
		printf("Pipes are not yet implemented\n");

		// free the memory allocated
		// for the pipe tree structure
		free_command(parsed_pipe);

		break;
	}
	}
}
