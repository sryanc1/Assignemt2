/******************************************************************************
 * Student Name    :
 * RMIT Student ID :
 * COURSE CODE     :
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include "helpers.h"
#include "repl.h"

/*****************************************************************************
 * convenience function to allocate a string and opy the data across to the new
 * memory.
 *****************************************************************************/
char* strdup(const char orig[])
{
        /* allocate space for the string */
        char* copy = (char*)malloc(strlen(orig) + 1);
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
        long len = strlen(line);
        long count = 0;
        long curcount = 0;

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
void * safemalloc(size_t size, long lineno){
	
	void * result = malloc(size);
	if (!result)
	{
		error_print("insufficient memory available, failed at line %l", lineno);
		exit(EXIT_FAILURE);
	}
	return result;
}
