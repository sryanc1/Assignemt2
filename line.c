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

/**************************************************************************
 * Constructor for the linked list - a paragraph of text is parsed 
 * at any one time. A pointer to the last node is passed back to the 
 * calling function. The function also takes in a pointer to the 
 * previous node and a pointer to the master list 
 **************************************************************************/
struct line_node * line_builder(char page_text[], 
								struct line_node * previous_node,
								struct line_list * thelist){
	
	long string_length;
	char * string;			 /*points to our lines wraped at 80 chars*/
	char new_line[1];		 /*used for blank lines - line only has \n char*/
	int num_lines;	
	/*set pointers for node and line structs*/ 
	struct line_node * current_node = NULL;
	struct line * line_struct = NULL;
	
	/*Establish the line numbering - if previous is NULL, this will
	 * ensure the first node is linked back to the head of the master struct*/
	if(previous_node == NULL)
	{
		num_lines = 0;
	}
	else
	{
		num_lines = thelist->num_lines;
	}
	
	/*check for blank lines - ie the first character is a new line*/
	if(page_text[FIRST_CHAR] == '\n')
	{
		/*Change the new line to a space*/
		new_line[FIRST_CHAR] = ' ';
		new_line[1] = '\0';
		string_length = strlen(new_line);
		
		if 
		/*insert the read data into a line struct and check if it fails*/
		((line_struct 
		= line_construct(new_line, string_length, num_lines)) == NULL
		/*connect the line struct to a node to 
		 * keep track and check failure*/
		||(current_node = line_node_construct(line_struct)) == NULL)
		{
			/*if either the line or the node cannot be constucted,
			 * close the file and 
			 * return false to the REPL to free the list*/				
			return NULL;
		}
		
		/*connect the first node to the master struct*/
		if(num_lines == FIRST_NODE)
		{
			if(thelist->head != NULL)
			{
				free(thelist->head);
			}
			thelist->head = current_node;
		}
		/*link the nodes together - the address of the first node 
		 * cannot be passed to a previous node as a previous does 
		 * not exist*/				
		else if(previous_node != NULL)
		{
			previous_node->next = current_node;
		}		
		
		/*update the master struct line count*/
		++thelist->num_lines;	

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
			= line_construct(string, string_length, num_lines)) == NULL
			/*connect the line struct to a node to 
			 * keep track and check failure*/
			||(current_node = line_node_construct(line_struct)) == NULL)
			{
				/*if either the line or the node cannot be constucted,
				 * close the file and 
				 * return false to the REPL to free the list*/
				return NULL;
			}	
			
			/*connect the first node to the master struct*/
			if(num_lines == FIRST_NODE)
			{
				if(thelist->head != NULL)
				{
					free(thelist->head);
				}
				thelist->head = current_node; 
			}
			
			/*link the nodes together - the address of the first node 
			 * cannot be passed to a previous node as a previous does 
			 * not exist*/				
			else if(previous_node != NULL)
			{
				previous_node->next = current_node;
			}
			
			/*update the master struct line count*/
			++thelist->num_lines;
			++num_lines;
			
			/*read from the previous token*/
			string = strtok(NULL, "\n");
			
			/*next node is redefined as the current 
			 * node to link then together*/
			previous_node = current_node;
		}
	}
	return current_node;
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
	
	for (i=0 ; i<linked_list_lines; ++i)
	{
		free(current_node->data->data);
		free(current_node->data);
		temp = current_node->next;
		free(current_node);
		current_node = temp;
	}
	
	free(thelist->file_name);
	thelist->head = NULL;
	thelist->file_name = NULL;
	thelist->num_lines = 0;	
}

/**************************************************************************
 * function to print entire list
 *************************************************************************/
BOOLEAN line_list_print(struct line_list * thelist){
	
	int linked_list_lines, i;
	struct line_node * current_node = thelist->head;
	linked_list_lines = thelist->num_lines;
	
	assert(thelist);
	
	for ( i=0; i<linked_list_lines; ++i)
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

/*****************************************************************************
 * a version of safemalloc
 *****************************************************************************/
void lines_renumber(struct line_list * thelist){
	
	int i;
	struct line_node * current_node = thelist->head;
	
	for (i=0; i<thelist->num_lines; ++i)
	{
		current_node->data->lineno = i;
		current_node = current_node->next;
	}
}
