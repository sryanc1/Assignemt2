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
	
	FILE * fp_read = NULL;		
	char page_text[BUFSIZ+1];/*holds a paragraph of text from the page/file*/
	
	/*set pointers for node and line structs*/ 
	struct line_node * current_node = NULL;
	struct line_node * previous_node = NULL;

	/*confirm a valid list is passed in*/
	assert(thelist);	
	
	/*check if a file pointer is passed in*/
	if (fname)
	{	
		/*ensure the file can be opened*/
		if(!(fp_read = fopen(fname, "r")))
		{
			error_print("unable to open the file %s", fname);
			return FALSE;
		}
	}
	
	/*initialise the master struct to 0 lines and point 
	 * to the head node, NULL for now*/
	thelist->num_lines = 0;
	thelist->head = current_node;
	
	/*assign the file name to the master struct*/
	thelist->file_name = strdup(fname);
	
	/*read a paragraph of text at a time*/
	while (fgets(page_text, BUFSIZ+1, fp_read) != NULL)
	{
		if((current_node = line_builder(page_text, previous_node, thelist))
			== NULL )
		{
			fclose(fp_read);
			error_print("Unable to load the file, please try again");
			free_nodes(thelist);
			free(thelist->file_name);
			return FALSE;
		}
		previous_node = current_node;
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
		if(!(fp_write = fopen(fname, "w")))
		{
			error_print("unable to open the file %s", fname);
			return FALSE;
		}
	}
	
	/*assign values to the variables*/
	current_node = thelist->head;
	linked_list_lines = thelist->num_lines;
	
	/*iterate over the list*/
	for ( i=0 ; i<linked_list_lines; ++i)
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
