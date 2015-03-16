#include <stdlib.h>
#include <assert.h>
#include "ast.h"

#define MAX_LIST_SIZE 1000

State* the_state_machine = NULL;


List makeList(void)
{
  List l = calloc(MAX_LIST_SIZE,sizeof(l[0]));
  assert(l!=NULL);

  return l;
}

void appendToList(void* data, List list)
{
  int i;

  for (i=0; i<MAX_LIST_SIZE; i++) {
    if (list[i]==NULL) {
      list[i] = data;
      return;
    }
  }
  assert("List if full\n"==NULL);
}

Transition makeTransition(const char* event, const char* state)
{
  Transition t = malloc(sizeof(struct Transition_R));
  assert(t!=NULL);

  t->event = event;
  t->state = state;

  return t;
}

Control makeControl(const char* line)
{
  Control c = malloc(sizeof(struct Control_R));
  assert(c!=NULL);

  c->line = line;

  return c;
}

State makeState(int is_initial, const char* name, Control* controls, Transition* transitions)
{
  State s = malloc(sizeof(struct State_R));
  assert(s!=NULL);

  s->is_initial = is_initial;
  s->name = name;
  s->controls = controls;
  s->transitions = transitions;

  return s;
}
