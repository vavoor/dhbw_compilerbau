/*
  bison -dtv parser.y
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.h"

void yyerror(const char* msg);
%}

%union {
  int number;
}

%token '(' ')'
%token<number> NUMBER
%type<number> expression

%start expressions

%%

expressions
  : expressions expr
  | /* empty */
  ;

%%

void yyerror(const char* msg)
{
  fprintf(stderr,"Error: %s\n",msg);
}


void fatal(const char* msg)
{
  fprintf(stderr,"Fatal error: %s\n",msg);
  exit(1);
}


int main(int argc, const char* argv[])
{
  yyparse();
  return 0;
}
