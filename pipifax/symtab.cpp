#include "symtab.hpp"

SymbolTable::SymbolTable()
{
  enterScope(); /* global scope always exists */
}

Node* SymbolTable::lookup(string* name)
{
  for (list<Scope*>::iterator l_it=m_symbols.begin(); l_it!=m_symbols.end(); l_it++) {
    Scope* scope = *l_it;
    Scope::iterator s_it = scope->find(*name);
    if (s_it!=scope->end()) {
      return s_it->second;
    }
  }
  return NULL;
}

bool SymbolTable::insert(string* name, Node* node)
{
  Scope* scope = m_symbols.front();
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
  m_symbols.push_front(scope);
}

void SymbolTable::leaveScope()
{
  Scope* scope = m_symbols.front();
  m_symbols.pop_front();
  delete scope;
}
