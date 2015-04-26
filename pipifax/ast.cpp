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
    if (!symtab.insertFunction(func->m_name,func)) {
      err(0,"Duplicate function %s",func->m_name->c_str());
    }
  }

  for (list<GlobalVarDeclaration*>::iterator var_it = m_variables.begin(); var_it!=m_variables.end(); var_it++) {
    GlobalVarDeclaration* var = *var_it;
    if (!symtab.insertVariable(var->m_name,var)) {
      err(0,"Duplicate global variable %s",var->m_name->c_str());
    }
  }

  for (list<FuncDefinition*>::iterator func_it = m_functions.begin(); func_it!=m_functions.end(); func_it++) {
    FuncDefinition* func = *func_it;
    func->resolve(&symtab);
  }
}

void FuncDefinition::resolve(SymbolTable* symtab)
{
  symtab->enterScope();

  symtab->insertVariable(m_return_value->m_name,m_return_value);

  for (list<ParamDeclaration*>::iterator it=m_params->begin(); it!=m_params->end(); it++) {
    ParamDeclaration* param = *it;
    symtab->insertVariable(param->m_name,param);
  }

  m_stmts->resolve(symtab);

  symtab->leaveScope();
}

void Block::resolve(SymbolTable* symtab)
{
  symtab->enterScope();

  for (list<LocalVarDeclaration*>::iterator it=m_declarations.begin(); it!=m_declarations.end(); it++) {
    LocalVarDeclaration* decl = *it;
    symtab->insertVariable(decl->m_name,decl);
  }

  for (list<Stmt*>::iterator it=m_stmts.begin(); it!=m_stmts.end(); it++) {
    Stmt* stmt = *it;
    stmt->resolve(symtab);
  }

  symtab->leaveScope();
}

void FunctionCall::resolve(SymbolTable* symtab)
{
  m_definition = symtab->lookupFunction(m_name);
  if (m_definition==NULL) {
    err(0,"Call of undefined function %s",m_name->c_str());
  }
}
