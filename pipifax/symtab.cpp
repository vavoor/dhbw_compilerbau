#include "symtab.hpp"

SymbolTable::SymbolTable()
{
  enterScope(); /* global scope always exists */
}

VarDeclaration* SymbolTable::lookupVariable(string* name)
{
  for (list<Scope*>::iterator l_it=m_variables.begin(); l_it!=m_variables.end(); l_it++) {
    Scope* scope = *l_it;
    Scope::iterator s_it = scope->find(*name);
    if (s_it!=scope->end()) {
      return s_it->second;
    }
  }
  return NULL;
}

bool SymbolTable::insertVariable(string* name, VarDeclaration* node)
{
  Scope* scope = m_variables.front();
  Scope::iterator it = scope->find(*name);
  if (it==scope->end()) {
    (*scope)[*name] = node;
    return true;
  }
  else {
    return false;
  }
}

void SymbolTable::enterScope()
{
  Scope* scope = new Scope();
  m_variables.push_front(scope);
}

void SymbolTable::leaveScope()
{
  Scope* scope = m_variables.front();
  m_variables.pop_front();
  delete scope;
}

FuncDefinition* SymbolTable::lookupFunction(string* name)
{
  map<string,FuncDefinition*>::iterator it = m_functions.find(*name);
  if (it!=m_functions.end()) {
    return it->second;
  }
  else {
    return NULL;
  }
}

bool SymbolTable::insertFunction(string* name, FuncDefinition* func)
{
  map<string,FuncDefinition*>::iterator it = m_functions.find(*name);
  if (it==m_functions.end()) {
    m_functions[*name] = func;
    return true;
  }
  else {
    return false;
  }
}
