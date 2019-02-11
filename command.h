/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include "fileio.h"
#include "line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMMAND_H
#define COMMAND_H
typedef BOOLEAN (*command_func)(const char[], struct line_list*);
enum command_type {
	CT_NEW = 'n',
	CT_READ = 'r',
	CT_WRITE = 'w',
	CT_PRINT = 'p',
	CT_INSERT = 'i',
	CT_DELETE = 'd',
	CT_SEARCH = 's',
	CT_REPLACE = 't',
	CT_HELP = 'h',
	CT_QUIT = 'q'
};

enum command_index {
	IND_NEW,
	IND_READ,
	IND_WRITE,
	IND_PRINT,
	IND_INSERT,
	IND_DELETE,
	IND_SEARCH,
	IND_REPLACE,
	IND_HELP,
	IND_QUIT
};

struct command {
	enum command_type type;
	command_func func;
};

#define NUM_COMMANDS 10
struct command * command_construct(enum command_type, BOOLEAN (*command_func)
(const char[], struct line_list*));
void commands_init(struct command commands[]);
BOOLEAN command_new(const char[], struct line_list*);
BOOLEAN command_read(const char[], struct line_list*);
BOOLEAN command_write(const char[], struct line_list*);
BOOLEAN command_print(const char[], struct line_list*);
BOOLEAN command_insert(const char[], struct line_list*);
BOOLEAN command_delete(const char[], struct line_list*);
BOOLEAN command_search(const char[], struct line_list*);
BOOLEAN command_replace(const char[], struct line_list*);
BOOLEAN command_help(const char[], struct line_list*);
BOOLEAN command_quit(const char[], struct line_list*);
#endif
