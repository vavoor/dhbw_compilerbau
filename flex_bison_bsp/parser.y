/*
  Translate with
  bison -dtv parser.y
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.h"
#include "ast.h"

void yyerror(const char* msg);
%}

%union {
  const char* s;
  int i;
  State state;
  Transition event;
  Control control;
  Control* controls;
  Transition* transitions;
}

%token T_INITIAL T_STATE T_ON T_GOTO T_CONTROL ':'
%token<s> T_IDENTIFIER

%type<i> opt_initial
%type<state> state
%type<event> event
%type<control> control
%type<controls> controls
%type<transitions> events

%start state_machine

%%

state_machine
  : states
  ;

states
  : states state
  | state
  ;

state
  : opt_initial T_STATE T_IDENTIFIER ':' controls events
    { $$ = makeState($1,$3,$5,$6); }
  ;

opt_initial
  : T_INITIAL
    { $$ = 1; }
  | /* empty */
    { $$ = 0; }
  ;

controls
  : controls control
    { $$ = $1; /* to do */ }
  | /* empty */
    { $$ = NULL; }
  ;

control
  : T_CONTROL T_IDENTIFIER
    { $$ = makeControl($2); }
  ;

events
  : events event
    { $$ = $1; /* to do */}
  | /* empty */
    { $$ = NULL; }
  ;

event
  : T_ON T_IDENTIFIER T_GOTO T_IDENTIFIER
    { $$ = makeTransition($2,$4); }
  ;

%%

void yyerror(const char* msg)
{
  fprintf(stderr,"Error: %s\n",msg);
  exit(1);
}

int main()
{
  yyparse();
  return 0;
}

