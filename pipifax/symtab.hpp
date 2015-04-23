#ifndef ID8346d44d_bc8a_493e_915d_69920c31cce0
#define ID8346d44d_bc8a_493e_915d_69920c31cce0

#include <list>
#include <map>
using namespace std;

class Node;

class SymbolTable {
private:
  typedef map<string*,Node*> Scope;
  list<Scope> m_symbols;

public:

};

#endif
