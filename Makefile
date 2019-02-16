 ##############################################################################
 # Student Name    : Craig RYAN
 # RMIT Student ID : s3555490
 # COURSE CODE     : COSC2138
 #
 # Startup code provided by Paul Miller for use in "Programming in C",
 # study period 4, 2018.
 ##############################################################################

HEADERS=command.h fileio.h helpers.h line.h main.h repl.h shared.h
OBJECTS=command.o fileio.o helpers.o line.o main.o repl.o
CC=gcc
CFLAGS=-ansi -Wall -pedantic
LFLAGS=

#default compilation target
all:lineedit

#link together the main executable
lineedit: $(OBJECTS)
	$(CC) $(LFLAGS) $^ -o $@

#compile each source file
%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

#compilation target for debugging: 
debug:CFLAGS+=-g
debug: clean all

#compilation target for address sanitisation: 
sanitize:CFLAGS+=-g -fsanitize=address
sanitize:LFLAGS+=-fsanitize=address
sanitize: clean all


.PHONY:clean
clean:
	rm -f lineedit *.o *~
