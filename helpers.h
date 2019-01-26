/******************************************************************************
 * Student Name    :
 * RMIT Student ID :
 * COURSE CODE     :
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

void * safemalloc(size_t, long);
void fold(char line[], long);
char* strdup(const char[]);
BOOLEAN empty_string(const char[]);
#endif
