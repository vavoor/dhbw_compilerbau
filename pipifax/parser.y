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
  : program global_var_decl
  | program func_def
  | /* empty */
  ;

global_var_decl
  : T_VAR ident type_specifier
  ;

func_def
  : T_FUNC indent '(' opt_param_list ')' opt_type_specifier block
  ;

opt_param_list
  : param_list
  | /* empty */
  ;

param_list
  : param_list ',' param_decl
  | param_decl
  ;

param_decl
  : indent param_type_specifier
  ;

param_type_specifier
  : type_specifier
  | '*' type_specifier
  | '[' ']' type_specifier /* TODO : is x[][] allowed? */
  ;

opt_type_specifier
  : type_specifier
  | /* empty */
  ;

block
  : '{' var_or_stmt_list '}'
  ;

var_or_stmt_list
  : var_or_stmt_list local_var_decl
  | var_or_stmt_list statement
  | /* empty */
  ;

local_var_decl
  : T_VAR ident type_specifier
  ;

statement
  : assignment
  | if_stmt
  | while_stmt
  | func_call
  ;

assignment
  : lvalue '=' expr
  ;

if_stmt
  : T_IF expr block
  | T_IF expr block T_ELSE block
  ;

type_specifier
  : T_INT
  | T_FLOAT
  | T_STRING
  | '[' T_INT_VALUE ']' type_specifier
  ;

ident
  : T_IDENT
  ;

%%

void yyerror(const char* msg)
{
    fatal(yylineno,msg);
}
