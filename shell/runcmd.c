#include "runcmd.h"

#include "handler.h"

int status = 0;
struct cmd *parsed_pipe = NULL;

// runs the command in 'cmd'
int
run_cmd(char *cmd)
{
	pid_t p;
	struct cmd *parsed;

	// if the "enter" key is pressed
	// just print the prompt again
	if (cmd[0] == END_STRING)
		return 0;

	// "history" built-in call
	if (history(cmd))
		return 0;

	// "cd" built-in call
	if (cd(cmd))
		return 0;

	// "exit" built-in call
	if (exit_shell(cmd))
		return EXIT_SHELL;

	// "pwd" built-in call
	if (pwd(cmd))
		return 0;

	// parses the command line
	parsed = parse_line(cmd);

	// forks and run the command
	if ((p = fork()) == 0) {
		// Sets the child PGID to be the same as the
		// child PID (foreground process).
		// This means that the sigchild handler won't
		// wait this process, since the PGID is not
		// the same as the parent PGID.
		if (parsed->type != BACK)
			setpgid(0, 0);

		// keep a reference
		// to the parsed pipe cmd
		// so it can be freed later
		if (parsed->type == PIPE)
			parsed_pipe = parsed;

		// Child process inherits the sigchild handler
		// and the alternative stack. Disabling it avoids
		// unwanted waits.
		disable_sigchild_handler();

		exec_cmd(parsed);
	}

	// stores the pid of the process
	parsed->pid = p;

	// background process special treatment
	if (parsed->type == BACK) {
		// Do not wait the back process, allowing
		// the shell to continue running.
		print_back_info(parsed);
	} else {
		// waits for the process to finish
		waitpid(p, &status, 0);

		print_status_info(parsed);
	}

	free_command(parsed);

	return 0;
}