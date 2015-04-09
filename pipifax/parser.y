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
  : T_FUNC ident '(' opt_param_list ')' opt_type_specifier block
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
  : ident param_type_specifier
  ;

param_type_specifier
  : type_specifier
  | '*' type_specifier
  | '*' '[' ']' type_specifier
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
  | func_call_stmt
  ;

assignment
  : lvalue '=' expr
  ;

if_stmt
  : T_IF expr block
  | T_IF expr block T_ELSE block
  ;

while_stmt
  : T_WHILE expr block
  ;

func_call_stmt
  : func_call
  ;

opt_arg_list
  : arg_list
  | /* empty */
  ;

arg_list
  : arg_list ',' expr
  | expr
  ;

expr
  : or_expr
  ;

or_expr
  : and_expr T_OR or_expr
  | and_expr
  ;

and_expr
  : comp_expr T_AND and_expr
  | comp_expr
  ;

comp_expr
  : add_expr '<' add_expr
  | add_expr T_LE add_expr
  | add_expr T_EQ add_expr
  | add_expr T_NE add_expr
  | add_expr T_GE add_expr
  | add_expr '>' add_expr
  | add_expr
  ;

add_expr
  : add_expr '+' mult_expr
  | add_expr '-' mult_expr
  | mult_expr
  ;

mult_expr
  : mult_expr '*' unary_expr
  | mult_expr '/' unary_expr
  | unary_expr
  ;

unary_expr
  : '-' unary_expr
  | '!' unary_expr
  | func_call
  | var_access
  | literal
  ;

func_call
  : ident '(' opt_arg_list ')' /* TODO : what about f()[5], i.e. is a function call an lvalue ?*/
  ;

var_access
  : lvalue
  ;

lvalue
  : lvalue '[' expr ']'
  | ident /* TODO : f()[7] = 4? */
  ;

literal
  : T_INT_VALUE
  | T_FLOAT_VALUE
  | T_STRING_VALUE
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
