/******************************************************************************
 * Student Name    :
 * RMIT Student ID :
 * COURSE CODE     :
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include "main.h"
#define NUMARGS 2
#define FILEARG argv[1]

int main(int argc, char* argv[]) {
	
		/* the array of commands that can be used in this program */
		struct command commands[NUM_COMMANDS];
		
		/* initialise the commands */
		commands_init(commands);
				
		/* if there are invalid arguments, display appropriate
		 * error messages and exit */
		if (argc > NUMARGS)
		{
			print_usage();
			return EXIT_FAILURE;
		}
		
		/* implement behaviour based on the number of arguments passed in */
		else if (argc < NUMARGS)
		{
			repl(commands, NULL);
		}
		else if (argc == NUMARGS)
		{
			repl(commands, FILEARG);
		}
		return EXIT_FAILURE;
}

/**
 * common function name used for explaining how to use the program
 **/
void print_usage(void) {
	
	const char error[] = {"There are to many file arguments." 
		"At most, only the text file is required"};
	error_print(error);
}
