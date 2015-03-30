#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

typedef void** List;
List makeList(void);
void appendToList(void* data, List);

struct ASTNode {
  enum { TRANSITION_NODE, CONTROL_NODE, STATE_NODE } type;
};

typedef struct Transition_R {
  struct ASTNode node;
  const char* event;
  const char* state;
} *Transition;
Transition makeTransition(const char* event, const char* state);

typedef struct Control_R {
  struct ASTNode node;
  const char* line;
} *Control;
Control makeControl(const char* line);

typedef struct State_R {
  struct ASTNode node;
  const char* name;
  Transition* transitions;
  Control* controls;
} *State;
State makeState(int is_initial, const char* name, Control* controls, Transition* transitions);

extern State* the_state_machine;
void generate_all(void);

#endif
