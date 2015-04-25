#include "ast.hpp"
#include "err.h"
#include "symtab.hpp"

Program* the_program;

IntType* IntType::m_singleton = NULL;
FloatType* FloatType::m_singleton = NULL;
StringType* StringType::m_singleton = NULL;
VoidType* VoidType::m_singleton = NULL;

void Program::resolve()
{
  SymbolTable symtab;

  for (list<FuncDefinition*>::iterator func_it = m_functions.begin(); func_it!=m_functions.end(); func_it++) {
    FuncDefinition* func = *func_it;
    if (!symtab.insert(func->m_name,func)) {
      err(0,"Duplicate name %s",func->m_name->c_str());
    }
  }

  for (list<GlobalVarDeclaration*>::iterator var_it = m_variables.begin(); var_it!=m_variables.end(); var_it++) {
    GlobalVarDeclaration* var = *var_it;
    if (!symtab.insert(var->m_name,var)) {
      err(0,"Duplicate name %s",var->m_name->c_str());
    }
  }

  for (list<FuncDefinition*>::iterator func_it = m_functions.begin(); func_it!=m_functions.end(); func_it++) {
    FuncDefinition* func = *func_it;
    if (!symtab.insert(func->m_name,func)) {
      err(0,"Duplicate name %s",func->m_name->c_str());
    }
  }
}
