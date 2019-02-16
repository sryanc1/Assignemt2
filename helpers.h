/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shared.h"
#ifndef HELPERS_H
#define HELPERS_H

#define DECIMAL 10

void * safemalloc(size_t);
int space_check(const char[]);
void fold(char line[], long);
char * strdup(const char[]);
BOOLEAN empty_string(const char[]);
BOOLEAN range_parser(const char remainder[], int * from, int * to, int num_lines);
#endif
