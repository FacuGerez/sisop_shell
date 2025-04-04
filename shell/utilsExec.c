#include "utilsExec.h"
#include "types.h"
#include "freecmd.h"

void run_exec_cmd(struct execcmd *cmds) {
	// im not searching in path the executable, i dont know how to do it
	execvp(cmds->argv[0], cmds->argv);
	// if i return from the execvp call, it means i have an error
	free_command((struct cmd *) cmds);
	_exit(-1);
}