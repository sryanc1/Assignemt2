/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include "command.h"
#include "helpers.h"
#include "repl.h"
/*****************************************************************************
 * copy into the commands array the structures to represeent each function to be
 * called by the system. The command_index enum defines the order of the command
 * structures in this array. Please note that your solution is meant to scale
 * and make it easy to update and maintain. An approach of a simple hardcoding
 * of the elements will get 0 marks. eg: you should use a loop or a mem*
 * function to do this initialisation.
 *****************************************************************************/
void commands_init(struct command commands[])
{
	int i;
	struct command * command_struct = NULL;
	
	enum command_type command_trigger[NUM_COMMANDS] = {
		CT_NEW, 
		CT_READ, 
		CT_WRITE, 
		CT_PRINT, 
		CT_INSERT, 
		CT_DELETE, 
		CT_SEARCH, 
		CT_REPLACE, 
		CT_HELP, 
		CT_QUIT
	};
		
	BOOLEAN (*command_func[NUM_COMMANDS])(const char[],struct line_list*) = {
		command_new,
		command_read,
		command_write,
		command_print,
		command_insert,
		command_delete,
		command_search,
		command_replace,
		command_help,
		command_quit
	};
				
	for (i=IND_NEW; i<=IND_QUIT; ++i)
	{
		command_struct = command_construct(command_trigger[i], 
											command_func[i]);
		/*dereference the command struct pointer 
		 * and add the struct to the array*/
		commands[i] = *command_struct;
	}
}

/*****************************************************************************
 * builds the structs for the commands array
 *****************************************************************************/
struct command * command_construct(enum command_type type, BOOLEAN 
(*command_func)(const char[], struct line_list*))
{
		struct command * command_struct;
		command_struct = safemalloc(sizeof(struct command));
		command_struct->type = type;
		command_struct->func = command_func;
		return command_struct;
}

/*****************************************************************************
 * implements the "new file" command. You should just set the line list to be
 * an empty list
 *****************************************************************************/
BOOLEAN command_new(const char remainder[], struct line_list* thelist)
{
		struct line * line_struct = NULL;
		struct line_node * current_node = NULL;
		struct line_node * next_node = NULL; 
		
		assert(thelist);
		
		if (strlen(remainder) > 0)
		{
				error_print(
				"trailing invalid chars found, please try again.\n");
				return FALSE;
		}	
			
		if (thelist->head != NULL)
		{
			free_nodes(thelist);
		}
		
		line_struct = line_construct(" ", 1,  0);
		
		current_node = line_node_construct(line_struct);
		
		current_node->next = next_node;
		thelist->num_lines = 0;
		thelist->head = current_node;        
		thelist->file_name = NULL;
		return TRUE;
}
/*****************************************************************************
 * handles a request to load a file into the linked list. The actual work of
 * loading the file should be done in fileio module.
 *****************************************************************************/
BOOLEAN command_read(const char remainder[], struct line_list* thelist)
{
		int first_char;
		char file_name[LINELEN];

		if (thelist->head != NULL)
		{
			free_nodes(thelist);
		}
		first_char = space_check(remainder);

		strcpy(file_name,remainder+first_char);

		if (!(load_file(file_name, thelist)))
		{	
			return FALSE;
		}
		return TRUE;
}

/*****************************************************************************
 * handles the saving of a file to disk. If no file is specified, the filename
 * that the file was specified under when loaded (if any) will be used.
 *****************************************************************************/
BOOLEAN command_write(const char remainder[], struct line_list* thelist)
{
		int copy_from;
		char fname[LINELEN];
		
		if(strlen(remainder) == 0 && thelist->file_name == NULL)
		{
			error_print("no file name has been provided - please try again");
			return FALSE;
		}
		else if(strlen(remainder) == 0)
		{
			strcpy(fname,thelist->file_name);
		} 
		else
		{
			copy_from = space_check(remainder);
			strcpy(fname, remainder+copy_from);
		}
		
		if(save_file(fname, thelist) == FALSE)
		{
			return FALSE;
		} 
		return TRUE;
}

/*****************************************************************************
 * handles the printing of lines from the "file" loaded into memory. We need to
 * handle 3 cases: when no range of lines is asked for, print out the whole
 * file. When a single number is provided after the 'p', print just the line
 * requested. When a range is requested, eg: p3-7 print out those lines. In all
 * cases, each line must be printed out as linenumber: text, eg:
 *
 * 3: this is the third line of the file
 *
 *****************************************************************************/
BOOLEAN command_print(const char remainder[], struct line_list* thelist)
{
		/*String pointers to point to parts of the arguments passed in*/
		char * part1 = NULL;
		char * part2 = NULL;
		char * endptr;	
		/*Ints to represent the print range*/
		int print_to, print_from, i;
		/*Int to represent the entire range - ie print bounds*/
		int linked_list_lines = thelist->num_lines;
		/*Pointer to the first node*/
		struct line_node * current_node = thelist->head;
		/*Somewhere to store the function arguments for manipulation*/
		char arg[LINELEN] ;
		
		/*Check if arguments are passed in - 
		 * ie the remainder string is >0 */
		if (strlen(remainder) == 0)
		{
			/*If no args - print entire list*/
			line_list_print(thelist);
			return TRUE;
		}
		
		/*Copy the remander to the args array for 
		 * manipulation as the remainder is constant*/
		strcpy(arg, remainder);
		
		/*Tokenise the first part of the argument - 
		 * "part1" becomes the handle*/
		part1 = strtok(arg, "-"); 
		
		/*Tokenise the second part - remains NULL if strtok fails here*/
		part2 = strtok(NULL, "-"); 
		
		/*Set errno to zero - used to check if "string to long" fails as 
		 * zero returns form this function on failure and is also a 
		 * valid return number*/
		errno = 0;
		
		/*Get the first number from the part1 string pointer*/
		print_from = strtol(part1, &endptr, DECIMAL);
		if (strlen(endptr) != 0 || errno != 0)
		{
			/*End ptr points any chars in the string - 
			 * there should be none */
			error_print("invalid arguments were passed in");
			return FALSE;
		}
		
		/*Check a second token has been set and repeat as above*/
		if (part2 == NULL)
		{
			/*If not token was set - ie only one line to print*/
			print_to = print_from;
		}
		else
		{	
			print_to = strtol(part2, &endptr, DECIMAL);
			if (strlen(endptr) != 0 || errno != 0)
			{
				error_print("invalid arguments were passed in");
				return FALSE;
			}
		}
		
		/*Check lines are not out of range*/
		if (print_from < 0 || print_to > linked_list_lines)
		{
			error_print("lines from %l to %l are out of range", 
						print_from, print_to);
			return FALSE;
		}
		
		/*Finally loop through each node */
        for(i=0; i<=linked_list_lines; ++i)
        {
			/*and print the requested nodes*/
			if (i >= print_from && i <= print_to)
			{
				line_print(current_node);
			}
			current_node = current_node->next;
		}
        return TRUE;
}

/*****************************************************************************
 * inserts one or more lines into linked list, starting at the new line number
 * specified by the user. The new line number specified by the user must be
 * greater than or equal to 1 (the first line) and less than or equal to the
 * count of lines in the file plus one (when the user chooses to append to the
 * end of the file).
 *****************************************************************************/
BOOLEAN command_insert(const char remainder[], struct line_list* thelist)
{
        printf("insert new file\n");
        return TRUE;
}

/*****************************************************************************
 * delete the lines at the line numbers specified in the remainder string.
 *****************************************************************************/
BOOLEAN command_delete(const char remainder[], struct line_list* thelist)
{
        printf("delete new file\n");
        return TRUE;
}

/*****************************************************************************
 * handles a request for a search across the list for a string specified. The
 * output here should be as spcified in the assignment specification and help
 * documentation provided in the sample executable.
 *****************************************************************************/
BOOLEAN command_search(const char remainder[], struct line_list* thelist)
{
        printf("search new file\n");
        return TRUE;
}

/*****************************************************************************
 * performs the find and replace algorithm, including the processing of
 * user input from the repl. Check that there are two '/' characters in the
 * string separated by other chars. Duplicate the string to perform tokenization
 * on it. Iterate over the list replacing each instance of the search string
 * with the replacement. Print out each replacement made. Please note that
 * you should do much / most of this work in functions you create yourself.
 *****************************************************************************/
BOOLEAN command_replace(const char remainder[], struct line_list* thelist)
{
        printf("replace new file\n");
        return TRUE;
}

/*****************************************************************************
 * handles the quitting of the program and freeing any allocated memory
 *****************************************************************************/
BOOLEAN command_quit(const char remainder[], struct line_list* thelist)
{
        printf("quit new file\n");
        return TRUE;
}

/*****************************************************************************
 * handles providing help to the user. I've provided the sourcecode for this
 * function as I don't want you to be spending time copying and pasting text as
 * there's little learning involved.
 *****************************************************************************/
BOOLEAN command_help(const char remainder[], struct line_list* thelist)
{
        (void)thelist;
        if (empty_string(remainder))
        {
                char help_string[] =
                    "You have the following options: \n\n"
                    "n - make a new file, discarding any currently loaded "
                    "text.\n"
                    "r - read a file into memory.\n"
                    "w - write a file out to disk.\n"
                    "p - print out lines from the file loaded into memory.\n"
                    "i - insert lines into the file loaded into memory.\n"
                    "d - delete lines from the file loaded into memory.\n"
                    "s - search the file loaded into memory.\n"
                    "h - display this help information.\n"
                    "q - quit the program.\n\n"
                    "If you want more information on one of these options, "
                    "simply enter 'h' followed by the option.\n";
                fold(help_string, LINELEN);
                normal_print("%s\n", help_string);
        }
        else
        {
                char* output = NULL;
                while (isspace(*remainder))
                {
                        ++remainder;
                }
                /* if the remainder at this point is more than one char (a
                 * command, then it's an invalid help request */
                if (strlen(remainder) > 1)
                {
                        error_print(
                            "trailing invalid chars in request for help.\n");
                        return FALSE;
                }
                /* handle each kind of possible help request */
                switch (tolower(*remainder))
                {
                        /* create a new file in memory */
                        case CT_NEW:
                        {
                                output = strdup(
                                    "n - no trailing argument. Creates a new "
                                    "empty file in memory. After doing this "
                                    "you can insert text on the first line. "
                                    "\n\n");
                        }
                        break;
                        /* read a file in from disk */
                        case CT_READ:
                        {
                                output = strdup(
                                    "r <path>, where path is the path to the "
                                    "file. Please note that no assumption is "
                                    "made about the path or the filename or "
                                    "file extension. Reads the file specified "
                                    "into memory and folds the file so that "
                                    "the maximul line length is 80 "
                                    "characters.");
                        }
                        break;
                        /* save the data in memory to a file on disk where the
                         * filename may be specified, if it is specified or
                         * should be taken from the file name that either it was
                         * loaded from or previously saved to */
                        case CT_WRITE:
                        {
                                output = strdup(
                                    "w <path> where path is the path to the "
                                    "file. Please note that no assumption is "
                                    "made about the path or the filename or "
                                    "file extension. Writes the file in memory "
                                    "to disk. If the lines in memory represent "
                                    "a file previously loaded into memory then "
                                    "the file name does not need to be "
                                    "provided.");
                        }
                        break;
                        /* print out a range of lines or the whole file if it is
                         * not specified */
                        case CT_PRINT:
                        {
                                output = strdup(
                                    "Prints out 1 or more lines of text. "
                                    "Can be invoked as: p on its own which "
                                    "will print out the whole file. "
                                    "Alternatively you can invoke this as pNUM "
                                    "where NUM is a valid line number and when "
                                    "invoked in this way, that line will be "
                                    "printed as NUM: text. For example: \n\n"
                                    "3: this is the text from line 3\n\n"
                                    "This can also be invoked as PNUM-NUM "
                                    "where NUM in both cases is a valid line "
                                    "number in the text file. Each line will "
                                    "be printed out in the same format as "
                                    "specified for printing out a single "
                                    "line.\n\n");
                        }
                        break;
                        case CT_INSERT:
                        {
                                output = strdup(
                                    "Inserts one or more lines into the file. "
                                    "You specify the line where you want the "
                                    "insert to start. For example: \n\n"
                                    "i3\n"
                                    "inserting at line 3: \n"
                                    "3: new text for line 3.\n"
                                    "4: new text for line 4.\n\n"
                                    "You can terminate the insertion of lines "
                                    "by pressing ctrl-d or [enter] on a new "
                                    "line. Please note that all new lines "
                                    "inserted will be folded at the 80 "
                                    "character mark, so if the line you enter "
                                    "is longer than 80 characters, it will "
                                    "become multiple lines in the list.\n\n");
                        }
                        break;
                        case CT_DELETE:
                        {
                                output = strdup(
                                    "Deletes one or more lines from the file "
                                    "loaded into memory. Obviously if there "
                                    "are no lines currently in the line list, "
                                    "nothing can be deleted.\n\n"
                                    "You enter d followed by either a single "
                                    "number such as 5 or a range of numbers "
                                    "such as 3-5. Please note that the numbers "
                                    "entered must be within the range of the "
                                    "list and the first number in the range "
                                    "must lower than or equal to the second "
                                    "number.\n\n");
                        }
                        break;
                        case CT_SEARCH:
                        {
                                output = strdup(
                                    "searches for the text search in each line "
                                    "of the file. Any leading spaces before the"
                                    " first non-space character are ignored "
                                    "however any paces after the beginning of "
                                    "the first word are considered part of the "
                                    "search because your search word might be "
                                    "multiword.\n\n");
                        }
                        break;
                        case CT_HELP:
                        {
                                output = strdup(
                                    "displays help for each option that can be "
                                    "invoked. Type h followed by the letter "
                                    "for the option you want to get help on. "
                                    "Eg: hp would give you help on the print "
                                    "command.\n\n");
                        }
                        break;
                        case CT_QUIT:
                        {
                                output = strdup(
                                    "Quits the program. Simply type the q "
                                    "character or press [enter] or ctrl-d at "
                                    "the prompt");
                        }
                        break;
                }
                if (output)
                {
                        fold(output, LINELEN);
                        normal_print(output);
                        free(output);
                }
        }
        return TRUE;
}
