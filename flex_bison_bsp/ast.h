#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

typedef struct Transition_R {
  const char* event;
  const char* state;
} *Transition;
Transition makeTransition(const char* event, const char* state);

typedef struct Control_R {
  const char* line;
} *Control;
Control makeControl(const char* line);

typedef struct State_R {
  const char* name;
  int is_initial;
  int transition_count;
  Transition* transitions;

  int control_count;
  Control* controls;
} *State;

State makeState(int is_initial, const char* name, Control* controls, Transition* transitions);

typedef struct StateMachine_R {
  int state_count;
  State* states;
} *StateMachine;

#endif
