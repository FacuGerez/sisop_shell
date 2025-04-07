#include "exec.h"
#include "utilsExec.h"
#include "types.h"

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
	// edit: lo hace lautaro asique tengo que esperar a eso

}


// opens the file in which the stdin/stdout/stderr
// flow will be redirected, and returns
// the file descriptor
//
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int
open_redir_fd(char *file, int flags)
{
	// Your code here

	return -1;
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
// - casting could be a good option
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
		run_exec_cmd(e);
		printf("Commands are not yet implemented\n");
		_exit(-1);
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
		//
		// To check if a redirection has to be performed
		// verify if file name's length (in the execcmd struct)
		// is greater than zero
		//
		// Your code here
		printf("Redirections are not yet implemented\n");
		_exit(-1);
		break;
	}

	case PIPE: {
		// pipes two commands
		//
		// Your code here
		struct pipecmd *pipe_cmd = (struct pipecmd *) cmd;
		struct cmd *left = pipe_cmd->leftcmd;
		struct cmd *right = pipe_cmd->rightcmd;

		int finish_status = EXIT_SUCCESS;
		int pipefd[2];
		if (pipe(pipefd) == -1) {
			perror("Error creating pipe");
			finish_status = EXIT_FAILURE;
			exit(finish_status);
		}

		pid_t fp = fork();
		if (fp == 0) {
			close(pipefd[0]);  // close read end of the pipe
			dup2(pipefd[1], STDOUT_FILENO);  // redirect stdout to pipe
			close(pipefd[1]);  // close write end of the pipe
			// child process
			exec_cmd(left);
		} else if (fp < 0) {
			perror("Error created a new process");
			finish_status = EXIT_FAILURE;
			close(pipefd[0]);  // close read end of the pipe
			close(pipefd[1]);  // close write end of the pipe
		} else {
			// parent process
			pid_t fp2 = fork();
			if (fp2 == 0) {
				close(pipefd[1]);  // close write end of the pipe
				dup2(pipefd[0],
				     STDIN_FILENO);  // redirect stdin to pipe
				close(pipefd[0]);  // close read end of the pipe
				// child process
				exec_cmd(right);
			} else if (fp2 < 0) {
				perror("Error created a new process");
				finish_status = EXIT_FAILURE;
				close(pipefd[0]);  // close read end of the pipe
				close(pipefd[1]);  // close write end of the pipe
				waitpid(fp, NULL, 0);
			} else {
				close(pipefd[0]);  // close read end of the pipe
				close(pipefd[1]);  // close write end of the pipe
				waitpid(fp, NULL, 0);
				waitpid(fp2, NULL, 0);
			}
		}
		// free the memory allocated
		// for the pipe tree structure
		free_command(parsed_pipe);
		parsed_pipe = NULL;
		exit(finish_status);
		break;
	}
	}
}
