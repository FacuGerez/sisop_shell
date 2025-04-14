#include "exec.h"

#include "freecmd.h"
#include "utils_exec.h"
#include "types.h"

typedef int pipe_t[2];

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
	for (int i = 0; i < eargc; i++) {
		char *arg = eargv[i];
		char key[BUFLEN];
		char value[BUFLEN];

		int eq_idx = block_contains(arg, '=');
		if (eq_idx == -1) {
			continue;
		}

		get_environ_key(arg, key);
		get_environ_value(arg, value, eq_idx);

		setenv(key, value, 1);  // 1 = overwrite if exists
	}
}

// executes a command - does not return
void
exec_cmd(struct cmd *cmd)
{
	switch (cmd->type) {
	case EXEC: {
		// spawns a command
		struct execcmd *e = (struct execcmd *) cmd;
    
    set_environ_vars(e->eargv, e->eargc);

		run_exec_cmd(e);

		break;
	}

	case BACK: {
		// runs a command in background
		struct backcmd *b = (struct backcmd *) cmd;

		exec_cmd(b->c);

		break;
	}

	case REDIR: {
		// changes the input/output/stderr flow
		struct execcmd *r = (struct execcmd *) cmd;
    
    set_environ_vars(r->eargv, r->eargc);

		// This case is the only one that might fail if the
		// file does not exist.
		if (strlen(r->in_file) &&
		    redirect_fd(STDIN_FILENO, r->in_file, O_RDONLY) == -1)
			exit(EXIT_FAILURE);

		// These are safe because if the file does not exist,
		// then a new file is created.
		if (strlen(r->out_file))
			redirect_fd(STDOUT_FILENO,
			            r->out_file,
			            O_RDWR | O_CREAT | O_TRUNC);

		if (strlen(r->err_file))
			redirect_fd(STDERR_FILENO,
			            r->err_file,
			            O_RDWR | O_CREAT | O_TRUNC);

		run_exec_cmd(r);

		break;
	}

	case PIPE: {
		// pipes two commands
		struct pipecmd *p = (struct pipecmd *) cmd;
		pid_t left_cpid, right_cpid;

		pipe_t pipe_fd;
		if (pipe(pipe_fd) == -1) {
			exit(EXIT_FAILURE);
		}

		if ((left_cpid = fork()) < 0) {
			exit(EXIT_FAILURE);
		}
		if (left_cpid == 0) {
			// Left process' stdout is redirected
			// to the pipe's write end.
			close(pipe_fd[READ]);
			dup2(pipe_fd[WRITE], STDOUT_FILENO);
			close(pipe_fd[WRITE]);
			exec_cmd(p->leftcmd);
		}

		if ((right_cpid = fork()) < 0) {
			exit(EXIT_FAILURE);
		}
		if (right_cpid == 0) {
			// Right process' stdin is redirected
			// to the pipe's read end.
			close(pipe_fd[WRITE]);
			dup2(pipe_fd[READ], STDIN_FILENO);
			close(pipe_fd[READ]);
			exec_cmd(p->rightcmd);
		}

		// Parent process does not need these
		// fds anymore.
		close(pipe_fd[READ]);
		close(pipe_fd[WRITE]);

		// Wait for both child processes.
		waitpid(left_cpid, NULL, 0);
		waitpid(right_cpid, NULL, 0);

		if (parsed_pipe != NULL) {
			free_command(parsed_pipe);
			parsed_pipe = NULL;
		}

		exit(EXIT_SUCCESS);
	}

	default: {
		exit(EXIT_FAILURE);
	}
	}
}
