/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include "fileio.h"
#include "repl.h"
#include "helpers.h"

/*****************************************************************************
 * loads the file specified from disk into a line_list.
 *****************************************************************************/
BOOLEAN load_file(const char fname[], struct line_list * thelist) {
	
	FILE * fp_read;		
	long string_length, line_count;
	char page_text[BUFSIZ+1];/*holds a paragraph of text from the page/file*/
	char * string;			 /*points to our lines wraped at 80 chars*/
	char new_line[1];		 /*used for blank lines - line only has \n char*/
	
	/*set pointers for node and line structs*/ 
	struct line_node * current_node = NULL;
	struct line_node * previous_node = NULL;
	struct line * line_struct = NULL;

	/*confirm a valid list is passed in*/
	assert(thelist);	
	
	/*initialise the master struct to 0 lines and point 
	 * to the head node, NULL for now*/
	thelist->num_lines = 0;
	thelist->head = current_node;
	
	/*initialise a line counter to keep track of lines read*/
	line_count = 0;
	
	/*check if a file pointer is passed in*/
	if (fname)
	{	
		/*ensure the file can be opened*/
		if(!fopen(fname, "r"))
		{
			error_print("unable to open the file %s", fname);
			return FALSE;
		}
		else 
		{
			fp_read = fopen(fname, "r");
		}
	}
	
	/*assign the file name to the master struct*/
	thelist->file_name = strdup(fname);
	
	/*read a paragraph of text at a time*/
	while (fgets(page_text, BUFSIZ+1, fp_read) != NULL)
	{
		/*check for blank lines - ie the first character is a new line*/
		if(page_text[FIRST_CHAR] == '\n')
		{
			/*Change the new line to a space*/
			new_line[0] = ' ';
			new_line[1] = '\0';
			string_length = strlen(new_line);
			
			if 
			/*insert the read data into a line struct and check if it fails*/
			((line_struct 
			= line_construct(new_line, string_length, line_count)) == NULL
			/*connect the line struct to a node to 
			 * keep track and check failure*/
			||(current_node = line_node_construct(line_struct)) == NULL)
			{
				/*if either the line or the node cannot be constucted,
				 * close the file and 
				 * return false to the REPL to free the list*/				
				fclose(fp_read);
				return FALSE;
			}
			
			/*connect the first node to the master struct*/
			if(line_count == FIRST_NODE)
			{
				thelist->head = current_node; 
			}
			
			/*link the nodes together - the address of the first node 
			 * cannot be passed to a previous node as a 
			 * previous does not exist*/			
			if(line_count != FIRST_NODE)
			{
				previous_node->next = current_node;
			}
			
			/*previous node is redefined as the current 
			 * node to link them together*/
			previous_node = current_node;
			
			/*update the master struct line count*/
			thelist->num_lines = line_count;
			
			/*increment the line counter to the next line*/
			++line_count;
		} 
		else 
		{
			/*add a new line character at a maximum 
			 * line width of LINELEN - 80 */
			fold(page_text, LINELEN);
		
			/*get the first non-blank line of the paragraph array*/
			string = strtok(page_text, "\n");
		
			/*keep reading the paragraph array intil the end*/
			while (string != NULL)
			{	
				
				/*length of string, info needed for the line struct*/
				string_length = strlen(string);
				
				if 
				/*insert the read data into a line 
				 * struct and check if it fails*/
				((line_struct 
				= line_construct(string, string_length, line_count)) == NULL
				/*connect the line struct to a node to 
				 * keep track and check failure*/
				||(current_node = line_node_construct(line_struct)) == NULL)
				{
					/*if either the line or the node cannot be constucted,
					 * close the file and 
					 * return false to the REPL to free the list*/
					fclose(fp_read);
					return FALSE;
				}	
				
				/*connect the first node to the master struct*/
				if(line_count == FIRST_NODE)
				{
					thelist->head = current_node; 
				}
				
				/*link the nodes together - the address of the first node 
				 * cannot be passed to a previous node as a previous does 
				 * not exist*/				
				if(line_count != FIRST_NODE)
				{
				previous_node->next = current_node;
				}
				
				/*next node is redefined as the current 
				 * node to link then together*/
				previous_node = current_node;
				
				/*update the master struct line count*/
				thelist->num_lines = line_count;
				
				/*increment the line counter to the next line*/
				++line_count;
				
				/*read from the previous token*/
				string = strtok(NULL, "\n");
			}
		}
	}
	current_node->next = NULL;
	fclose(fp_read);
	return TRUE;
}

/*****************************************************************************
 * saves the file represented by the line_list to the filename specified
 *****************************************************************************/
BOOLEAN save_file(const char fname[], const struct line_list* thelist) {
		
	FILE * fp_write;
	int linked_list_lines, i;
	struct line_node * current_node = NULL;
	
	assert(thelist);
	
	/*check if a file pointer is passed in*/
	if (fname)
	{	
		/*ensure the file can be opened*/
		if(!fopen(fname, "w"))
		{
			error_print("unable to open the file %s", fname);
			return FALSE;
		}
		else 
		{
			fp_write = fopen(fname, "w");
		}
	}
	
	/*assign values to the variables*/
	current_node = thelist->head;
	linked_list_lines = thelist->num_lines;
	
	/*iterate over the list*/
	for ( i=0 ; i<=linked_list_lines; ++i)
	{
		/*add lines of data to the specified file*/
		if(fprintf(fp_write, "%s\n", current_node->data->data) <0)
		{
			fclose(fp_write);
			return FALSE;
		}
		/*move to the next node*/
		current_node = current_node->next;
	}
	fclose(fp_write);
	return TRUE;
}
