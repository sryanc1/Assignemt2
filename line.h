/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include "shared.h"
#include "helpers.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef LINE_H
#define LINE_H
struct line {
        char* data;
        long len;
        long lineno;
};

struct line_node {
        struct line* data;
        struct line_node* next;
};

struct line_list {
        struct line_node* head;
        char* file_name;
        long num_lines;
};

/**
 * function prototypes for allocating, manipulating and freeing the line_list
 * and its components go here.
 **/
void line_list_init(struct line_list*);
struct line * line_construct(char * data, long len, long lineno);
struct line_node * line_node_construct(struct line * line_struct);
BOOLEAN line_print(struct line_node * current_node);
BOOLEAN line_list_print(struct line_list * thelist);
int error_print(const char[], ...);
void free_nodes(struct line_list * thelist);
#endif
