#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

static FILE* input;
static int la; /* This is a single character lookahead */

/**
  Prints an error message and terminates program execution with an error code.
*/
static void fatal_error(const char* msg)
{
  fprintf(stderr,"Error: %s - Aborting!\n",msg);
  exit(1);
}

/**
  Reads the next character from the input into the lookahead variable.
*/
static void next(void)
{
  la = fgetc(input);
}

/**
  Skips spaces. On return the lookahead is a non-space character or EOF.
*/
void skip_spaces(void)
{
  while (isspace(la)) {
    next();
  }
}

/**
  Expects the lookahead to be one of the characters in matches. It consumes the
  lookahead.
  If lookahead does not match, a fatal error is raised.
*/
static void match(const char* matches)
{
  if (strchr(matches,la)==NULL) {
    fatal_error("Unexpected character");
  }
  next();
}

/**
  Opens the input file filename and initializes the parser.
  Raises a fatal error if the file could not be opened.
*/
static void open_grammar(const char* filename)
{
  input = fopen(filename,"r");
  if (input==NULL) {
    fatal_error("Cannot open input file");
  }
  next();
  skip_spaces();
}

static void close_grammar(void)
{
  fclose(input);
  input = NULL;
}

/*************************************************************************
                    PUT THE GRAMMAR RULES AFTER HERE
**************************************************************************/


/*************************************************************************/

/**
  Parses the input file filename. If the input has sytax errors, an error message is
  printed and the program is terminated with an error code.
*/
void parse_grammar(const char* filename)
{
  open_grammar(filename);
  close_grammar();
}
