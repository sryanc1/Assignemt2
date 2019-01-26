/******************************************************************************
 * Student Name    :
 * RMIT Student ID :
 * COURSE CODE     :
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include "line.h"

/**
 * Functions for allocation, manipulation and freeing of the line_list go here
 **/

/**************************************************************************
 * list_init() - sets all list values to zero.
 *************************************************************************/
void line_list_init(struct line_list * list){
	
	/*Confirm a list is passed in*/
	assert(list);
	
	/*set values to zero*/
	memset(list, 0, sizeof(struct line_list));	
	
}

/**************************************************************************
 * constructor for line node
 *************************************************************************/
struct line_node * line_node_construct(struct line * line_struct, 
							struct line_node * previous, long lineno){
	/*allocate memory for the node*/
	struct line_node * node = safemalloc(sizeof(struct line_node), lineno);
	/*assign the variables data to the struct*/
	node->data = line_struct;
	node->next = previous;
	return node;
}

/**************************************************************************
 * constructor for the line. 
 *************************************************************************/
struct line * line_construct(char * data, long len, long lineno){
	
	/*allocate memory for the line*/
	struct line * line_data = safemalloc(sizeof(struct line), lineno);
	/*assign the variable data to the struct*/
	strcpy(line_data->data, data);
	line_data->len = len;
	line_data->lineno = lineno;
	return line_data;
}

