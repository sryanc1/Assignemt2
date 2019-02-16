/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include "helpers.h"
#include "repl.h"

/*****************************************************************************
 * convenience function to allocate a string and copy the data across to the new
 * memory.
 *****************************************************************************/
char* strdup(const char orig[])
{
        /* allocate space for the string */
        char* copy = (char*)malloc((strlen(orig)*sizeof(char)) + 1);
        /* check that it succeeded */
        if (!copy)
        {
                perror("malloc failed");
                return NULL;
        }
        /* copy the string and record it */
        strcpy(copy, orig);
        return copy;
}

/*****************************************************************************
 * inserts newline characters to wrap lines at less than the number of
 * characters specified.
 *****************************************************************************/
void fold(char line[], long width)
{
        long len, count, curcount;
        len = 0;
        len = strlen(line);
        count = 0;
        curcount = 0;

        while (count < len)
        {
                ++count;
                ++curcount;
                if (count == len)
                {
                        return;
                }
                if (line[count] == '\n')
                {                        
                        curcount = 0;
                }
                if (curcount == width)
                {
                        /* subtract from the current string address until we get
                         * to a whitespace character */
                        while (!isspace(line[count]))
                        {
                                --count;
                        }
                        /* replace the whitespace character with a newline
                         * character */
                        line[count] = '\n';
                        curcount = 0;
                        /* we start examining the rest ofthe string starting at
                         * the next character after the character that we
                         * replaced
                         */
                }
        }
}

/*****************************************************************************
 * checks a string array for the first non space char - returns its position.
 *****************************************************************************/
int space_check(const char string[])
{
        long count;
        long len = strlen(string);
        for (count = 0; count < len; ++count)
        {
                if (!isspace(string[count]))
                {
                        return count;
                }
        }
        return NOT_FOUND;
}

/*****************************************************************************
 * checks that a string only has spaces.
 *****************************************************************************/
BOOLEAN empty_string(const char string[])
{
        long count;
        long len = strlen(string);
        for (count = 0; count < len; ++count)
        {
                if (!isspace(string[count]))
                {
                        return FALSE;
                }
        }
        return TRUE;
}
/*****************************************************************************
 * a version of safemalloc
 *****************************************************************************/
void * safemalloc(size_t size){
	
	void * result = malloc(size);
	if (!result)
	{
		error_print("insufficient memory available to load the file\n");
		return NULL;
	}
	return result;
}

/*****************************************************************************
 * a function to parse the range of lines to be maniplulated.
 *****************************************************************************/
BOOLEAN range_parser(const char remainder[], int * from, int * to, int num_lines){
	
	/*String pointers to point to parts of the arguments passed in*/
	char * arg = NULL; 
	char * part1 = NULL;
	char * part2 = NULL;
	char * endptr;	
	int num_from, num_to;
		
	if (space_check(remainder) == NOT_FOUND)
	{
		error_print("invalid arguments were passed in");
		return FALSE;
	}
		
	/*Copy the remander to the args array for 
	 * manipulation as the remainder is constant*/
	arg = malloc(strlen(remainder)+1* sizeof(char));
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
	num_from = strtol(part1, &endptr, DECIMAL);
	if (space_check(endptr) != NOT_FOUND || errno != 0)
	{
		/*End ptr points any chars in the string - 
		 * there should be none */
		error_print("invalid arguments were passed in");
		free(arg);
		return FALSE;
	}
	
	if (num_from >= num_lines)
	{
		/*Checking that the range is does note begin after the last node */
		if(num_lines == 0)
		{
			num_from = num_lines;
		}
		else
		{
			num_from = num_lines-1;
		}
	}
	
	/*Check a second token has been set and repeat as above*/
	if(part2 == NULL)
	{
		/*If no token was set*/
		num_to = num_from;
	}
	else
	{	
		num_to = strtol(part2, &endptr, DECIMAL);
		
		if(num_to >= num_lines)
		{
			num_to = num_lines-1;
		}
		
		if(num_to < num_from || 
		space_check(endptr) != NOT_FOUND || errno != 0)
		{
			error_print("invalid arguments were passed in");
			free(arg);
			return FALSE;
		}
	}	
	
	*to = num_to;
	*from = num_from;
	free(arg);
	return TRUE;
}
