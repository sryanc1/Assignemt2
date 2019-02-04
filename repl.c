/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include "repl.h"
/*****************************************************************************
 *
 * commands to be implemented for our repl:
 * - read(r): read a file into memory. Lines when read in must be wrapped at or
 *   before 80 characters.
 *
 * - write(w): write the lines in memory to a file - a file must have been
 *   previously loaded.
 *
 * - print(p): print out the lines of a file. Optionally a range of lines can
 *   be provided, either a single line number or a range where the first
 *   number must be lower than the second however both numbers must be within
 *   the range of the lines in the file loaded.
 *
 * - insert(i): specified by i followed by an integer within the range of 1
 *   to the number of lines in the file plus 1.
 *
 * - delete (d): specified by the letter d followed by a single number or 2
 *   numbers separated by a minus sign where the first number must be less
 *   than or equal to the second number.
 *
 * - search (s): specified by the letter s followed by a work or a phrase. The
 *   program will iterate over all the lines of the file and print out each
 *   line that contains the matching phrase. All leading spaces between the
 *   command and the first letter shall be ignored.
 *
 *   After printing out 25 matches, the user will be asked if they wish to
 *   continue and if they do, the next 25 lines that match will be printed.
 *
 * - replace (t): specified by the letter t followed by a slash followed by the
 *   search term followed by a slash followed by what to replace it with.
 *   The output in this case should be either a report stating no matches were
 *   found or a list of all matches found in the format of the original line
 *   then an arrow then the line after the replacements have been done. Please
 *   note that the replacement algorithm is a bonus requirement. You can get
 *   full marks without implementing this.
 *
 *   The prompt which must be used to indicate that the program is ready for
 *   input is the > character. Any errors should have clear output so the user
 *   knows what they need to do to correct it. If errors occur (the command in
 *   question returns FALSE), the prompt should be a question mark.
 *
 *   All commands may have 0 or more spaces between the command and the
 *   argument. For
 *****************************************************************************/

/*****************************************************************************
 * performs buffer clearning for the program. It is static as all i/o should
 * be implemented in this module.
 *****************************************************************************/
static void read_rest_of_line(void) {

   int c;

   /* read until the end of the line or end-of-file */   
   while ((c = fgetc(stdin)) != '\n' && c != EOF)
      ;

   /* clear the error and end-of-file flags */
   clearerr(stdin);
}

/*****************************************************************************
 * implements the command interpreter for this application. It displays the
 * current prompt (based on whether the last action succeeded or not then
 * reads input then decides which command to invoke, then invokeds that command.
 *****************************************************************************/
void repl(const struct command commands[], char filename[]) {
	
	/*Define a variable to hold the user selection*/
	char selection[LINELEN+EXTRACHARS];
	char * select;
	/*Define a temp file in the case where no file is passed in*/
	FILE * temp;
	char tempfile[] = "temp.txt";
	
	/*Define and allocated memory for the master struct list*/
	struct line_list* thelist;
	if (!(thelist = malloc(sizeof(struct line_list))))
	{
		error_print("unable to allocate memory to run the program");
		exit(EXIT_FAILURE);
	}
	
	/*Create a temp file in the case where no file is passed in.
	 *This will be passed to "load file" and instantiate a single node*/
	if(!filename)
	{	
		temp = fopen(tempfile, "w");
		fprintf(temp, "\n");
		fclose(temp);
		filename = tempfile;
	}
	
	/*Load the file define by the user*/
	if(!load_file(filename, thelist))
	{
		error_print("unable to load the file");
		free_list(thelist);
		exit(EXIT_FAILURE);
	}
	
	/*Print the list*/
	line_list_print(thelist);
	
	/*Save the list back to the input file*/
	if(!save_file(filename, thelist))
	{
		error_print("unable to save the file");
		free_list(thelist);
		exit(EXIT_FAILURE);
	}
	
	/*Dysplay a prompt*/
	printf("> ");
	
	/*Use fgets to populate the selection array
	 *Continue until user hits return on an empty line or ctrl-D*/
	while ((select = fgets(selection, LINELEN+1, stdin)) != NULL && 
	selection[0] != '\n') 
	{
		/*Dysplay a prompt*/
		printf("> ");	
			
		/*Check if the entry is to long*/
		if (selection[strlen(selection)-1] != '\n')
		{
			read_rest_of_line();
			select = NULL;
			error_print("to many characters entered");
		}	
	}
	free_list(thelist);	
}  

/*****************************************************************************
 * acts as a proxy to printf. All output sent here will go to normal output via
 * "stdout"
 *****************************************************************************/
int normal_print(const char format[], ...) {
        va_list va_args;
        int output_chars;
        
        /* initialise the variable args */
        va_start(va_args, format);
        
        /* pass them to printf */
        output_chars = vprintf(format, va_args);
        
        /* finish with processing the variable args */
        va_end(va_args);
        
        /* return the number of chars written out */
        return output_chars;
}

/*****************************************************************************
 * acts as a proxy to printf, except the output gets sent to stderr, the
 * error file pointer. . All output sent here will go to normal output via
 * "stdout"
 *****************************************************************************/
int error_print(const char format[], ...) {
        va_list va_args;
        int output_chars;
        
        /* initialise the variable args */
        va_start(va_args, format);
        
        /* output preamble to an error message: */
        output_chars = fprintf(stderr, "Error: ");
        
        /* print the error message */
        output_chars += vfprintf(stderr, format, va_args);
        output_chars += fprintf(stderr, "\n");
        return output_chars;
}
