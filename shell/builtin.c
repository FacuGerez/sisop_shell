#include "builtin.h"
#include "parsing.h"
#include <string.h>


#define EXIT_COMMAND_KEYWORD "exit"
#define CD_COMMAND_KEYWORD "cd"

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
	char *right = split_line(cmd, ' ');

	if (strcmp(cmd, CD_COMMAND_KEYWORD) != 0) {
		return 0;
	}

    bool goHome = strcmp(right, "") == 0;
	int open;
	printf("%s", right);
	if (goHome) {
		open = chdir(right); // deberia pasar el path de home
	} else {
		open = chdir(right);
	}

	//getcwd();

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
	// Your code here

	return 0;
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
