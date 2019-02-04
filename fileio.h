/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "shared.h"
#include "line.h"
#include "helpers.h"

#ifndef FILEIO_H
#define FILEIO_H
#define FIRST_NODE 0
#define FIRST_CHAR 0
BOOLEAN load_file(const char[], struct line_list*);
BOOLEAN save_file(const char[], const struct line_list*);
void free_list(struct line_list * thelist);
#endif
