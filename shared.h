/******************************************************************************
 * Student Name    : Craig Ryan
 * RMIT Student ID : s3555490
 * COURSE CODE     : COSC2138
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 4, 2018.
 *****************************************************************************/

#ifndef SHARED_H

#define SHARED_H
/**
 * definition of the boolean type for the application 
 **/
typedef enum {
        FALSE,
        TRUE
} BOOLEAN;

/**
 * required for string handling. The length of a line and the extra characters
 * appended to a string successfully read by fgets.
 **/
#define LINELEN 80
#define EXTRACHARS 2

#endif
