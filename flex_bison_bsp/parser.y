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
  State* states;
  Transition event;
  Transition* events;
  Control control;
  Control* controls;
}

%token T_INITIAL T_STATE T_ON T_GOTO T_CONTROL ':'
%token<s> T_IDENTIFIER

%type<i> opt_initial
%type<state> state
%type<states> states
%type<event> event
%type<events> events
%type<control> control
%type<controls> controls

%start state_machine

%%

state_machine
  : states
    { the_state_machine = $1; }
  ;

states
  : states state
    { $$ = $1; appendToList($2,(List)$1); }
  | state
    { $$ = (State*) makeList(); appendToList($1,(List)$$); }
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
    { $$ = $1; appendToList($2,(List)$$); }
  | /* empty */
    { $$ = (Control*) makeList(); }
  ;

control
  : T_CONTROL T_IDENTIFIER
    { $$ = makeControl($2); }
  ;

events
  : events event
    { $$ = $1; appendToList($2,(List)$$); }
  | /* empty */
    { $$ = (Transition*) makeList(); }
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
  if (yyparse()==0) {
    generate_all();
  }

  return 0;
}

