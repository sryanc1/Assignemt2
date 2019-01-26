/******************************************************************************
 * Student Name    :
 * RMIT Student ID :
 * COURSE CODE     :
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
BOOLEAN load_file(const char fname[], struct line_list* thelist) {
		
	FILE * fp_read;		
	/*Confirm a valid list is passed in*/
	assert(thelist);
	/*Set pointers for node and line structs*/ 
	struct line_node * node = NULL;
	struct line_node * next = NULL;
	struct line * line_struct = NULL;
	/*initialise the master struct to 0 lines and point to the head node
	 * not malloced yet. I think it needs to be done before its passed in?*/
	thelist->num_lines = 0;
	thelist->head = node;
	/*allocate memory to read in a line from text file*/
	char line_data[LINELEN];
	/*initialise a line counter to keep track of lines read*/
	long line_count = 0;
	/*Check is a file pointer is passed in*/
	if (!fname)
	{
		theList->filename = "something.txt" /*?? no memory allocated for this 
											- how to do it?*/
		/*create an empty line list*/
		line_struct = line_construct(line_data, LINELEN, line_count);
		node = line_node_construct(line_struct, next, line_count);
		/*should end up with "thelist" pointing to this node and "this node" pointing to the line created here?*/
		return TRUE;
	}
	else if (fname)
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
	/*keep reading the file intil the end*/
	while (fgets(line_data, LINELEN, fp_read))
	{
		/*insert the read data into a line struct*/
		line_struct = line_construct(line_data, LINELEN, line_count); 
		/*connect the line struct to a node to keep track of lines*/
		node = line_node_construct(line_struct, next, line_count);	
		/*next node is redefined as the current node to link then together*/
		next = node;
		/*increment the line counter to the next line*/
		++line_count;
		/*update the master pointer line count*/
		thelist->num_lines = line_count;
	}

	return TRUE;
}

/*****************************************************************************
 * saves the file represented by the line_list to the filename specified
 *****************************************************************************/
BOOLEAN save_file(const char fname[], const struct line_list* thelist) {
		return FALSE;
}
