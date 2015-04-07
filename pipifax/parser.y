%{
#include <string>
using namespace std;

#include <string.h>
#include "parser.tab.h"
#include "lex.yy.h"
#include "err.h"


void yyerror(const char* msg);
%}

%union {
  const char* s;
  int i;
  double d;
}

%token T_VAR T_FUNC T_IF T_WHILE
%token T_INT T_FLOAT T_STRING
%token<s> T_IDENT
%token<i> T_INT_VALUE
%token<d> T_FLOAT_VALUE
%token<s> T_STRING_VALUE

%start program

%%

program
    :
    ;

%%

void yyerror(const char* msg)
{
    fatal(yylineno,msg);
}
