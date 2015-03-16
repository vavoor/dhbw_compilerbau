#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"

#define MAX_LIST_SIZE 1000

State* the_state_machine = NULL;
State initial_state = NULL;

List makeList(void)
{
  List l = calloc(MAX_LIST_SIZE,sizeof(l[0]));
  assert(l!=NULL);

  // fprintf(stderr,"Creating list %p\n",l);

  return l;
}

void appendToList(void* data, List list)
{
  int i;

  for (i=0; i<MAX_LIST_SIZE; i++) {
    if (list[i]==NULL) {
      list[i] = data;
      // fprintf(stderr,"Appending at index %d to list %p\n",i,list);
      return;
    }
  }
  assert("List if full\n"==NULL);
}

Transition makeTransition(const char* event, const char* state)
{
  Transition t = malloc(sizeof(struct Transition_R));
  assert(t!=NULL);

  t->node.type = TRANSITION_NODE;
  t->event = event;
  t->state = state;

  // fprintf(stderr,"Transition %s->%s created\n",event,state);

  return t;
}

Control makeControl(const char* line)
{
  Control c = malloc(sizeof(struct Control_R));
  assert(c!=NULL);

  c->node.type = CONTROL_NODE;
  c->line = line;

  // fprintf(stderr,"Control %s created\n",line);

  return c;
}

State makeState(int is_initial, const char* name, Control* controls, Transition* transitions)
{
  State s = malloc(sizeof(struct State_R));
  assert(s!=NULL);

  s->node.type = STATE_NODE;
  s->name = name;
  s->controls = controls;
  s->transitions = transitions;

  if (is_initial) {
    if (initial_state!=NULL) {
      fprintf(stderr,"Only one initial state is allowed\n");
      exit(1);
    }
    initial_state = s;
  }

  // fprintf(stderr,"State %s created.\n",name);

  return s;
}


static void generate_state(State s)
{
  int i;

  printf("    case state_%s: {\n",s->name);

  for (i=0; s->controls[i]!=NULL; i++) {
    printf("      set_output_line(line_%s);\n",s->controls[i]->line);
  }

  printf("      switch (get_event()) {\n");

  for (i=0; s->transitions[i]!=NULL; i++) {
    printf("      case event_%s:\n",s->transitions[i]->event);
    printf("        state = state_%s;\n",s->transitions[i]->state);
  }

  printf("      }\n");
}

void generate_all(void)
{
  if (initial_state==NULL) {
    fprintf(stderr,"No initial state defined!\n");
    exit(1);
  }

  printf("void run_state_machine(void)\n{\n");
  printf("  int state = state_%s;\n\n",initial_state->name);
  printf("  while (1) {\n");
  printf("    switch (state) {\n");

  int i;
  for (i=0; the_state_machine[i]!=NULL; i++) {
    generate_state(the_state_machine[i]);
  }

  printf("    }\n");
  printf("  }\n");
  printf("}\n\n");
}

