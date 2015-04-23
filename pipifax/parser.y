%{
#include <string>
#include <list>
#include <map>

using namespace std;

#include <string.h>
#include <stdlib.h>

#include "lex.yy.h"
#include "parser_inc.hpp"
#include "err.hpp"


void yyerror(const char* msg);
%}

%union {
  char* s;
  int i;
  double d;
  string* str;
}

%token '(' ')' '[' ']' '{' '}'
%token '+' '-' '*' '/' '=' '<' '>' '!' ','
%token T_VAR T_FUNC T_IF T_ELSE T_WHILE
%token T_INT T_FLOAT T_STRING
%token T_LE T_GE T_EQ T_NE T_AND T_OR
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
