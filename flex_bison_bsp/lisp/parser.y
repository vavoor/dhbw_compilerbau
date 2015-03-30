%{
#include "lex.yy.h"

static void yyerror(const char* msg);
%}

%token '(' ')'
%token SYMBOL
%token NUMBER

%start lists

%%

lists
  : lists list
  | /* empty */
  ;

list
  : '(' elements ')'
  ;

elements
  : elements element
  | /* empty */
  ;

element
  : SYMBOL
  | NUMBER
  | list
  ;

%%

static void yyerror(const char* msg)
{
  fprintf(stderr,"Error in line %d: %s\n",yylineno,msg);
}

int main(int argc, const char* argv[])
{
  yyparse();
  return 0;
}
