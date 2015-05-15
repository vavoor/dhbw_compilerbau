#ifndef ID8346d44d_bc8a_493e_915d_69920c31cce0
#define ID8346d44d_bc8a_493e_915d_69920c31cce0

#include <string>
#include <list>
#include <map>
using namespace std;

class VarDeclaration;
class FuncDefinition;

class SymbolTable {
private:
  typedef map<string,VarDeclaration*> Scope;
  list<Scope*> m_variables;
  map<string,FuncDefinition*> m_functions;

public:
  SymbolTable();

  VarDeclaration* lookupVariable(string* name);

  /* Returns true if the element has been inserted */
  bool insertVariable(VarDeclaration* var);

  void enterScope();
  void leaveScope();

  FuncDefinition* lookupFunction(string* name);
  bool insertFunction(FuncDefinition* func);
};

#endif
