/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
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
struct line_node * line_node_construct(struct line * line_struct){
	
	struct line_node * node;
	/*Confirm a struct is passed in*/
	assert(line_struct);
		
	/*allocate memory for the node*/
	node = safemalloc(sizeof(struct line_node));
	
	if (!node)
	{
		return NULL;
	}
	/*assign the variables data to the struct*/
	node->data = line_struct;
	return node;
}

/**************************************************************************
 * constructor for the line. 
 *************************************************************************/
struct line * line_construct(char * string, long len, long lineno){
	
	struct line * line_data;
	/*Confirm a string is passed in*/
	assert(string);	
	
	/*allocate memory for the line*/
	line_data = safemalloc(sizeof(struct line));
	
	if(!line_data)
	{
		return NULL;
	}

	/*assign the variable data to the struct*/
	line_data->data = strdup(string);
	line_data->len = len;
	line_data->lineno = lineno;
	return line_data;
}

/*****************************************************************************
 * freeing allocated memory - frees memory from the furthest point 
 * (ie the line data) then works back to each node. Each nodes "next"
 * is assigned a temp position then the node is freed before moving 
 * to the next. 
 *****************************************************************************/
void free_nodes(struct line_list * thelist){
	
	int linked_list_lines, i;
	struct line_node * temp = NULL;
	struct line_node * current_node = thelist->head;
	linked_list_lines = thelist->num_lines;
	
	assert(thelist);
	
	for (i=0 ; i<=linked_list_lines; ++i)
	{
		free(current_node->data->data);
		free(current_node->data);
		temp = current_node->next;
		free(current_node);
		current_node = temp;
	} 	
}

/**************************************************************************
 * function to print entire list
 *************************************************************************/
BOOLEAN line_list_print(struct line_list * thelist){
	
	int linked_list_lines, i;
	struct line_node * current_node = thelist->head;
	linked_list_lines = thelist->num_lines;
	
	assert(thelist);
	
	for ( i=0; i<=linked_list_lines; ++i)
	{
		if (line_print(current_node) == FALSE)
		{
			error_print("your file is too big");
			return FALSE;
		}
		current_node = current_node->next;
	} 	
	return TRUE;
}
	
	
/**************************************************************************
 * function to print a sinle node line - prints a space before the 
 * text part in order to line them up. Prints up to 999,999 lines 
 *************************************************************************/
BOOLEAN line_print(struct line_node * current_node){
	
	long lineno = current_node->data->lineno;
	char * lines = current_node->data->data;
	
	assert(current_node);
	
	if (lineno < DECIMAL)
	{
		printf("%lu:    ", lineno);
	} 
	else if (lineno < (DECIMAL * DECIMAL))
	{
		printf("%lu:   ", lineno);
	}
	else if (lineno < (DECIMAL * DECIMAL * DECIMAL))
	{
		printf("%lu:  ", lineno);
	}
	else if (lineno < (DECIMAL * DECIMAL * DECIMAL * DECIMAL))
	{
		printf("%lu: ", lineno);
	}
	else
	{
		printf("WOW, thats a really big file - try breaking it in to"
		"smaller parts");
		return FALSE;
	}
	printf("%s \n", lines);
	return TRUE;
} 
