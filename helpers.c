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


