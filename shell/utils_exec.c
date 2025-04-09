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
