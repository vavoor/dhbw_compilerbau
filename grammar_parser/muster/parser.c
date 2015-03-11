#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

static FILE* input;
static int la; /* This is a single character lookahead */
static int linenumber;
static int debug = 0;

/**
  Prints an error message and terminates program execution with an error code.
*/
static void fatal_error(const char* msg)
{
  fprintf(stderr,"Line %d: Error: %s - Aborting!\n",linenumber,msg);
  exit(1);
}

/**
  Reads the next character from the input into the lookahead variable.
*/
static void next(void)
{
  la = fgetc(input);
  if (debug) {
    fputc(la,stderr);
  }

  if (la=='\n') {
    linenumber++;
  }
}

/**
  Skips spaces. On return the lookahead is a non-space character or EOF.
*/
static void skip_spaces(void)
{
  while (isspace(la)) {
    next();
  }
}

/**
  Skips everything until (and including) the end of line
*/
static void skip_comment(void)
{
  while (la!=EOF && la!='\n') {
    next();
  }
  next();
}

/**
  Skips spaces and comments
*/
static void skip(void)
{
  skip_spaces();
  while (la=='#') {
    skip_comment();
    skip_spaces();
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
  Consumes the lookahead if it is one of the matches, otherwise leaves it unchanged.
*/
static void match_optional(const char* matches)
{
  if (strchr(matches,la)!=NULL) {
    next();
  }
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
  skip();

  linenumber = 1;
}

static void close_grammar(void)
{
  fclose(input);
  input = NULL;
}

/*************************************************************************
                    PUT THE GRAMMAR RULES AFTER HERE
**************************************************************************/

/*
  Grammar:

  grammar -> rule*
  rule -> ID '::' rule '.'
  expr -> concat ('|' concat)*
  concat -> postfix+
  postfix -> primary ('*'|'+'|'?')?
  primary -> '(' rule ')' | ID | STRING
  ID -> Letter (Letter|Digit|'_')*
  STRING -> "..."
*/

/* Forward Declaration */
static void p_expr(void);

static int firstOfPostfix(int c)
{
  return c=='('||c=='\"'||isalpha(c);
}

static void p_STRING(void)
{
  int delimiter = la;
  next();
  while (la!=delimiter && la!=EOF) {
    next();
  }
  next();
  skip();
}

static void p_ID(void)
{
  if (isalpha(la)||la=='_') {
    while (isalnum(la)||la=='_') {
      next();
    }
  }
  else {
    fatal_error("Identifier expected");
  }
  skip();
}

static void p_primary(void)
{
  if (la=='(') {
    next();
    skip();
    p_expr();
    match(")");
    skip();
  }
  else if (la=='\"') {
    p_STRING();
  }
  else if (isalpha(la)) {
    p_ID();
  }
  else {
    fatal_error("Identifier, string or parenthesis expected");
  }
}

static void p_postfix(void)
{
  p_primary();
  match_optional("*+?");
  skip();
}

static void p_concat(void)
{
  p_postfix();
  while (firstOfPostfix(la)) {
    p_postfix();
  }
}

static void p_expr(void)
{
  p_concat();
  while (la=='|') {
    next();
    skip();
    p_concat();
  }
}

static void p_rule(void)
{
  p_ID();
  skip();
  match(":");
  match(":");
  skip();
  p_expr();
  match(".");
  skip();
}

static void p_grammar(void)
{
  while (la!=EOF) {
    p_rule();
  }
}

/*************************************************************************/

/**
  Parses the input file filename. If the input has sytax errors, an error message is
  printed and the program is terminated with an error code.
*/
void parse_grammar(const char* filename)
{
  open_grammar(filename);
  p_grammar();
  close_grammar();
}
