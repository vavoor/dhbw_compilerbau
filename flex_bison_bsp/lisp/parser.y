%{
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "lex.yy.h"

static void yyerror(const char* msg);
%}

%union {
  const char* string;
  int number;
  ListElement element;
  List list;
}

%token '(' ')'
%token<string> SYMBOL
%token<number> NUMBER

%type<element> element
%type<list> list lists elements

%start file

%%

file
  : lists
  ;

lists
  : lists list
    { $$ = $1; append(make_list_element($2),$$); }
  | /* empty */
    { $$ = make_list(); }
  ;

list
  : '(' elements ')'
    { $$ = $2; }
  ;

elements
  : elements element
    { $$ = $1; append($2,$$); }
  | /* empty */
    { $$ = make_list(); }
  ;

element
  : SYMBOL
    { $$ = make_symbol_element($1); }
  | NUMBER
    { $$ = make_number_element($1); }
  | list
    { $$ = make_list_element($1); }
  ;

%%

static void yyerror(const char* msg)
{
  fprintf(stderr,"Error in line %d: %s\n",yylineno,msg);
}

int main(int argc, const char* argv[])
{
  yylineno = 0;
  if (argc==2) {
    yyin = fopen(argv[1],"r");
    if (yyin==NULL) {
      yyerror("Cannot open input file");
      exit(1);
    }
    yyparse();
    fclose(yyin);
  }
  return 0;
}
