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
	struct command command_struct;
	
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
	
	/*Loop through the array of command triggers and function names - add these
	 * to a struct and add the struct to the array of structs*/			
	for (i=IND_NEW; i<=IND_QUIT; ++i)
	{
		command_struct = command_construct(command_trigger[i], 
											command_func[i]);
		/*dereference the command struct pointer 
		 * and add the struct to the array*/
		commands[i] = command_struct;
	}
}

/*****************************************************************************
 * builds the structs for the commands array
 *****************************************************************************/
struct command command_construct(enum command_type type, BOOLEAN 
(*command_func)(const char[], struct line_list*))
{
		struct command command_struct;
		command_struct.type = type;
		command_struct.func = command_func;
		return command_struct;
}

/*****************************************************************************
 * implements the "new file" command. You should just set the line list to be
 * an empty list
 *****************************************************************************/
BOOLEAN command_new(const char remainder[], struct line_list* thelist)
{
		
		struct line_node * current_node = NULL;
		struct line_node * next_node = NULL; 
		
		assert(thelist);
		
		/*Check there are no file args passed in*/
		if (strlen(remainder) > 0)
		{
				error_print(
				"trailing invalid chars found, please try again.\n");
				return FALSE;
		}	
		
		/*Check the list is note allready blank*/	
		if (thelist->head->data != NULL)
		{
			/*free_nodes() frees everything except the list*/
			free_nodes(thelist);
		}
		else 
		{
			error_print("a new blank list is already loaded");
			return FALSE;
		}
		
		if (!(current_node = safemalloc(sizeof(struct line_node))))
		{
			return FALSE;
		}
		
		current_node->data = NULL;
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
		char * file_name;

		if (thelist->head != NULL)
		{
			free_nodes(thelist);
		}
		
		/*Create some space to copy the file name into - excluding any 
		 * leading space chars*/
		if(!(file_name = safemalloc(strlen(&remainder[space_check(remainder)])
							+1*sizeof(char))))
		
		{
			return FALSE;
		}
		
		/*Copy the remainder (ie function args)*/
		strcpy(file_name,&remainder[space_check(remainder)]);

		/*Pass this to the load_file function*/
		if (!(load_file(file_name, thelist)) || file_name == NULL)
		{	
			error_print("failed to read the file");
			return FALSE;
		}
		
		free(file_name);
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
		
		/*Check a current / valid list is initialised*/
		if(thelist->head == NULL)
		{
			error_print("A file is not currently loaded."
						" Please load a file and try again");
			return FALSE;
		}
		
		if(strlen(remainder) == 0 && thelist->file_name == NULL)
		{
			error_print("no file name has been provided - please try again");
			return FALSE;
		}
		
		else if(strlen(remainder) > 0 && space_check(remainder) == NOT_FOUND)
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
		/*Ints to represent the print range*/
		int print_to, print_from, i;
		/*Int to represent the entire range - ie print bounds*/
		int num_lines = thelist->num_lines;
		/*Pointer to the first node*/
		struct line_node * current_node = thelist->head;
		
		if (thelist->head == NULL)
		{
			error_print("A file is not currently loaded."
						" Please load a file and try again");
			return FALSE;
		}
		
		/*Check if arguments are passed in - 
		 * ie the remainder string is >0 */
		if (strlen(remainder) == 0)
		{
			/*If no args - print entire list*/
			line_list_print(thelist);
			return TRUE;
		}
			
		if(range_parser(remainder, &print_from, &print_to, num_lines) == FALSE)
		{
			return FALSE;
		}
		
		/*Check lines are not out of range*/
		if (print_from < 0 || print_to > num_lines)
		{
			printf("lines from %i to %i are out of range\n", print_from, print_to);
			return FALSE;
		}
		
		/*Finally loop through each node */
        for(i=0; i<num_lines; ++i)
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
		/*Somewhere to store the user insertion text data*/
		char * select = NULL;
		char * text = NULL;
		char * more_text = NULL;
		char selection[BUFSIZ];
		/*The point to insert new lines from and a counter variable*/
		int insert_from, i = 0, num_lines = thelist->num_lines;
		/*Nodes start at the head / holders for the list when it is broken
		 * for insertion and to link them together again*/
		struct line_node * current_node = thelist->head;
		struct line_node * next_node = NULL;
		struct line_node * node_link = NULL;
        
        /*Check a current / valid list is initialised*/
		if(thelist->head == NULL)
		{
			error_print("A file is not currently loaded."
						" Please load a file and try again");
			return FALSE;
		}
		
		/*Check if arguments are passed in - 
		 * ie the remainder string is >0 */
		if (strlen(remainder) == 0)
		{
			error_print("please enter the line number"
						" to begin inserting from");
			return FALSE;
		}
		
		if(range_parser(remainder, &insert_from, &insert_from, num_lines) == FALSE)
		{
			return FALSE;
		}	
		
		/*Display the insertion prompt*/
		printf("%i:  ", insert_from);
		
		/*Continue until the user hits return on a blank line or
		 * ctl-D is entered*/
		while ((select = fgets(selection, BUFSIZ, stdin)) != NULL && 
		selection[0] != '\n')
		{
			printf("%i:  ", insert_from);
			
			/*Check if the entry is to long*/
			if (selection[strlen(selection)-1] != '\n')
			{
				select = NULL;
				error_print("to many characters entered");
			}
			
			/*Fisrt line is a special case as we need to set the amount of 
			 * memory and copy the user entered text here. This allows for 
			 * multiple paragraphs but ends the function if a '\n' is 
			 * entered on a blank line or ctrl-D*/
			if(i==0)
			{
				/*Allocate and initialise memory - user text is copied here*/
				if(!(text = calloc(strlen(selection)+1, sizeof(char))))
				{
					error_print("unable to allocate memory for new lines");
					return FALSE;
				}
			}
			else
			{
				/*Allocate memory for the old and new user entered text here.
				 * This adds space for the new paragrph and the old one*/
				if(!(text = realloc(more_text, (strlen(text) + 
				strlen(selection)+1) * sizeof(char))))
				{
					free(text);
					error_print("unable to allocate memory for new lines");
					return FALSE;
				}
			}
			/*Copy the user text into the new memory location*/
			more_text = strcat(text, selection);
			
			/*Counter is used to ditinguish the first block of user text*/
			++i;
		}
		
		/*New line required for returning function - select will == NULL
		 * if the user enters ctrl-D*/
		if (select == NULL)
		{
			printf("\n");
		}		
		
		/*Loop until we find the line to insert the text from - if inserting
		 * from line zero - this will fail! eg: never enters the for loop.
		 * - handled further down*/
		for (i=0; i<insert_from; ++i)
		{
			/*The actual insert is always 1 less than the user input*/
			if(i == insert_from-1 )
			{
				/*Break the linked list and assign a place holder*/
				next_node = current_node->next;
				/*The new node is assiged here - and the line builder is
				 * invoked - many nodes maybe added at this point 
				 * - the final node is returned*/
				node_link = line_builder(more_text, current_node, thelist);
			}
			current_node = current_node->next;
		}
		
		/*Link the list together again*/
		if(insert_from == 0)
		{
			/*Used if inserting from the begining of a blank / new list*/
			node_link = line_builder(more_text, next_node, thelist);
			
			/*the line_builder() will link this to the master struct and 
			 * return the last node that is built to be relinked to the 
			 * list*/
			node_link->next = current_node;
			
		}
		else if (insert_from == thelist->num_lines-1)
		{
			/*Used if insertion is at the end of the list*/
			node_link->next = NULL;
		}
		else
		{
			/*Used to link the place holder and the new nodes together*/
			node_link->next = next_node;
		}
		
		/*Renumber the lines*/
		lines_renumber(thelist);
		
		free(text);
		return TRUE;
}

/*****************************************************************************
 * delete the lines at the line numbers specified in the remainder string.
 *****************************************************************************/
BOOLEAN command_delete(const char remainder[], struct line_list* thelist)
{
		struct line_node * temp = NULL;
		struct line_node * next_node = NULL;
		struct line_node * current_node = thelist->head;
		char blank[] = "";
		/*Ints to represent the delete range*/
		int delete_to, delete_from;
		int i, num_lines = thelist->num_lines;
		
		if (thelist->head == NULL)
		{
			error_print("A file is not currently loaded."
						" Please load a file and try again");
			return FALSE;
		}
		
		/*Check if arguments are passed in - 
		 * ie the remainder string is >0 */
		if (strlen(remainder) == 0)
		{
			error_print("please try again and enter the phrase you wish"
						" to search");
			return FALSE;
		}
			
		if(range_parser(remainder, &delete_from, &delete_to, num_lines) == FALSE)
		{
			return FALSE;
		}
		
		if(delete_from == FIRST_NODE && delete_to == num_lines-1)
		{
			command_new(blank, thelist);
			return TRUE;
		}
				
		/*Loop until we find the node to delect*/
		for (i=0; i<=num_lines-1; ++i)
		{
			if (i == delete_from-1)
			{
				/*Break the linked list and assign a place holder*/
				next_node = current_node;
			}
			
			/*The actual deletion of nodes and lines*/
			if (i >= delete_from && i <= delete_to)
			{
				free(current_node->data->data);
				free(current_node->data);
				temp = current_node->next;
				free(current_node);
				current_node = temp;
				--thelist->num_lines;
			}
			
			/*If the deletion lines dont macth - move to the next node*/
			else 
			{
				current_node = current_node->next;
			}
			
			/*Link the first node after the last deletion back to the list.
			 * Note that the first node cannot be the head*/
			if(i == delete_to && delete_from != 0)
			{
				next_node->next = current_node;
			}
			
			/*Here the head node is reassigned to the master struct if 
			 * deletion begins from the first node*/
			else if(delete_from == 0 && i == delete_to)
			{
				thelist->head = current_node;
			}
			
			/*If the last node is deleted, the places holders next 
			 * becomes null*/
			else if(delete_to == num_lines && i == num_lines)
			{
				next_node->next = NULL;
			}
		}
		
		/*Renumber the lines*/
		lines_renumber(thelist);
		
				
		return TRUE;
}

/*****************************************************************************
 * handles a request for a search across the list for a string specified. The
 * output here should be as spcified in the assignment specification and help
 * documentation provided in the sample executable.
 *****************************************************************************/
BOOLEAN command_search(const char remainder[], struct line_list* thelist)
{
		char * arg = NULL;
		struct line_node * current_node = thelist->head;
		int i;
        
		if (thelist->head == NULL)
		{
			error_print("A file is not currently loaded."
						" Please load a file and try again");
			return FALSE;
		}
		
		/*Check if arguments are passed in - 
		 * ie the remainder string is >0 */
		if (strlen(remainder) == 0)
		{
			error_print("please try again and enter the text you would"
						" like to search for");
			return FALSE;
		}		
		
		/*Copy the remander to the args array for 
		 * manipulation as the remainder is constant*/
		arg = malloc(strlen(&remainder[space_check(remainder)])+1* sizeof(char));
		strcpy(arg, &remainder[space_check(remainder)]);  
		
		for(i=0; i<thelist->num_lines; ++i)
		{
			if(strstr(current_node->data->data, arg))
			{
				line_print(current_node);
			}
			current_node = current_node->next;
		}
			    
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
		if (thelist->head !=NULL)
		{
			free_nodes(thelist);
		}
	
		free(thelist->file_name);
		free(thelist);	
		exit(EXIT_SUCCESS);
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
