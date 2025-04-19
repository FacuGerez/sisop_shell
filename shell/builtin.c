#include "builtin.h"
#include "defs.h"
#include "string.h"
#include "utils.h"

#define EXIT_COMMAND_KEYWORD "exit"
#define CD_COMMAND_KEYWORD "cd"
#define PWD_COMMAND_KEYWORD "pwd"

// returns true if the 'exit' call
// should be performed
//
// (It must not be called from here)
int
exit_shell(char *cmd)
{
	return strcmp(cmd, EXIT_COMMAND_KEYWORD) == 0;
}

// returns true if "chdir" was performed
//  this means that if 'cmd' contains:
// 	1. $ cd directory (change to 'directory')
// 	2. $ cd (change to $HOME)
//  it has to be executed and then return true
//
//  Remember to update the 'prompt' with the
//  	new directory.
//
// Examples:
//  1. cmd = ['c','d', ' ', '/', 'b', 'i', 'n', '\0']
//  2. cmd = ['c','d', '\0']
int
cd(char *cmd)
{
	// TODO: Refactorizar.
	if (!(strncmp(cmd, CD_COMMAND_KEYWORD, 2) == 0 &&
	      (cmd[2] == '\0' || cmd[2] == ' '))) {
		return 0;
	}

	const char *path = split_line(cmd, ' ');

	const bool go_home = strcmp(path, "") == 0;
	int open;
	if (go_home) {
		open = chdir(getenv("HOME"));  // TODO: Revisar getenv.
	} else {
		open = chdir(path);
	}

	if (open != 0) {
		status = 2;
		return open;
	}
	char directory[BUFLEN];
	const char *actual = getcwd(directory, sizeof(directory));
	strncpy(prompt, actual, PRMTLEN - 1);

	return open;
}

// returns true if 'pwd' was invoked
// in the command line
//
// (It has to be executed here and then
// 	return true)
int
pwd(char *cmd)
{
	if (strncmp(cmd, PWD_COMMAND_KEYWORD, 3) != 0) {
		return 0;
	}

	char directory[BUFLEN];
	if (!getcwd(directory, sizeof(directory))) {
		status = 2;
	} else {
		printf("%s\n", directory);
	}

	return 1;
}

// returns true if `history` was invoked
// in the command line
//
// (It has to be executed here and then
// 	return true)
int
history(char *cmd)
{
	// Your code here

	return 0;
}
