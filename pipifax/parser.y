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
  Program* program;
  FuncDefinition* func_def;
  GlobalVarDeclaration* global_var_decl;
  LocalVarDeclaration* local_var_decl;
  ParamDeclaration* param_decl;
  Type* type_specifier;
  list<VarDeclaration*>* var_decls;
  Block* block;
  LValue* lvalue;
  Stmt* stmt;
  FunctionCall* func_call;
  Expr* expr;
  list<Expr*>* exprs;
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

%type<program> elements
%type<func_def> func_def
%type<global_var_decl> global_var_decl
%type<param_decl> param_decl
%type<local_var_decl> local_var_decl
%type<type_specifier> type_specifier param_type_specifier opt_type_specifier
%type<var_decls> opt_param_list param_list
%type<block> block var_or_stmt_list
%type<stmt> statement assignment if_stmt while_stmt func_call_stmt
%type<expr> expr or_expr and_expr comp_expr add_expr mult_expr unary_expr var_expr literal
%type<func_call> func_call
%type<exprs> opt_arg_list arg_list
%type<lvalue> lvalue
%type<str> ident

%start program

%%

program
  : elements
    { the_program = $1; }
  ;

elements
  : elements global_var_decl
    { $$ = $1; $$->m_variables.push_back($2); }
  | elements func_def
    { $$ = $1; $$->m_functions.push_back($2); }
  | /* empty */
    { $$ = new Program(); }
  ;

global_var_decl
  : T_VAR ident type_specifier
    { $$ = new GlobalVarDeclaration($2,$3); }
  ;

func_def
  : T_FUNC ident '(' opt_param_list ')' opt_type_specifier block
    { $$ = new FuncDefinition($2,(list<ParamDeclaration*>*)$4,$6,$7); }
  ;

opt_param_list
  : param_list
  | /* empty */
    { $$ = new list<VarDeclaration*>(); }
  ;

param_list
  : param_list ',' param_decl
    { $$ = $1; $$->push_back($3); }
  | param_decl
    { $$ = new list<VarDeclaration*>(); $$->push_back($1); }
  ;

param_decl
  : ident param_type_specifier
    { $$ = new ParamDeclaration($1,$2); }
  ;

param_type_specifier
  : type_specifier
  | '*' type_specifier
    { $$ = new ReferenceType($2); }
  | '*' '[' ']' type_specifier
    { $$ = new ReferenceType(new DimensionlessArrayType($4)); }
  ;

opt_type_specifier
  : type_specifier
  | /* empty */
    { $$ = VoidType::getInstance(); }
  ;

block
  : '{' var_or_stmt_list '}'
    { $$ = $2; }
  ;

var_or_stmt_list
  : var_or_stmt_list local_var_decl
    { $$ = $1; $$->m_declarations.push_back($2); }
  | var_or_stmt_list statement
    { $$ = $1; $$->m_stmts.push_back($2); }
  | /* empty */
    { $$ = new Block(); }
  ;

local_var_decl
  : T_VAR ident type_specifier
    { $$ = new LocalVarDeclaration($2,$3); }
  ;

statement
  : assignment
  | if_stmt
  | while_stmt
  | func_call_stmt
  ;

assignment
  : lvalue '=' expr
    { $$ = new AssignmentStmt($1,$3); }
  ;

if_stmt
  : T_IF expr block
    { $$ = new IfStmt($2,$3,new Block()); } /* TODO : empty block instead of NULL? */
  | T_IF expr block T_ELSE block
    { $$ = new IfStmt($2,$3,$5); }
  ;

while_stmt
  : T_WHILE expr block
    { $$ = new WhileStmt($2,$3); }
  ;

func_call_stmt
  : func_call
    { $$ = new FunctionCallStmt($1); }
  ;

opt_arg_list
  : arg_list
  | /* empty */
    { $$ = new list<Expr*>(); }
  ;

arg_list
  : arg_list ',' expr
    { $$ = $1; $$->push_back($3); }
  | expr
    { $$ = new list<Expr*>(); $$->push_back($1); }
  ;

expr
  : or_expr
  ;

or_expr
  : and_expr T_OR or_expr
    { $$ = new OrExpr($1,$3); }
  | and_expr
  ;

and_expr
  : comp_expr T_AND and_expr
    { $$ = new AndExpr($1,$3); }
  | comp_expr
  ;

comp_expr
  : add_expr '<' add_expr
    { $$ = new LessExpr($1,$3); }
  | add_expr T_LE add_expr
    { $$ = new LessEqualExpr($1,$3); }
  | add_expr T_EQ add_expr
    { $$ = new EqualExpr($1,$3); }
  | add_expr T_NE add_expr
    { $$ = new NotEqualExpr($1,$3); }
  | add_expr T_GE add_expr
    { $$ = new GreaterEqualExpr($1,$3); }
  | add_expr '>' add_expr
    { $$ = new GreaterExpr($1,$3); }
  | add_expr
  ;

add_expr
  : add_expr '+' mult_expr
    { $$ = new AddExpr($1,$3); }
  | add_expr '-' mult_expr
    { $$ = new SubExpr($1,$3); }
  | mult_expr
  ;

mult_expr
  : mult_expr '*' unary_expr
    { $$ = new MultExpr($1,$3); }
  | mult_expr '/' unary_expr
    { $$ = new DivExpr($1,$3); }
  | unary_expr
  ;

unary_expr
  : '-' unary_expr
    { $$ = new NegExpr($2); }
  | '!' unary_expr
    { $$ = new NotExpr($2); }
  | func_call
    { $$ = $1; }
  | var_expr
  | literal
  ;

func_call
  : ident '(' opt_arg_list ')' /* TODO : what about f()[5], i.e. is a function call an lvalue ?*/
    { $$ = new FunctionCall($1,$3); }
  ;

var_expr
  : lvalue
    { $$ = new VariableExpr($1); }
  ;

lvalue
  : lvalue '[' expr ']'
    { $$ = new ArrayAccess($1,$3); }
  | ident
    { $$ = new VarAccess($1); }
  /* TODO : f()[7] = 4? */
  ;

literal
  : T_INT_VALUE
    { $$ = new IntLiteral($1); }
  | T_FLOAT_VALUE
    { $$ = new FloatLiteral($1); }
  | T_STRING_VALUE
    { $$ = new StringLiteral($1); free($1); }
  ;

type_specifier
  : T_INT
    { $$ = IntType::getInstance(); }
  | T_FLOAT
    { $$ = FloatType::getInstance(); }
  | T_STRING
    { $$ = StringType::getInstance(); }
  | '[' T_INT_VALUE ']' type_specifier
    { $$ = new ArrayType($4,$2); }
  ;

ident
  : T_IDENT
    { $$ = new string($1); free($1); }
  ;

%%

void yyerror(const char* msg)
{
    fatal(yylineno,msg);
}
