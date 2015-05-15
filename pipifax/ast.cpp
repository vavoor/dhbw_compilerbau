#include "ast.hpp"
#include "err.hpp"
#include "symtab.hpp"

Program* the_program;

IntType IntType::s_instance;
FloatType FloatType::s_instance;
StringType StringType::s_instance;
VoidType VoidType::s_instance;

void Program::resolve()
{
  SymbolTable symtab;

  for (list<FuncDefinition*>::iterator func_it = m_functions.begin(); func_it!=m_functions.end(); func_it++) {
    FuncDefinition* func = *func_it;
    if (!symtab.insertFunction(func)) {
      errmsg("Duplicate function %s",func->m_name->c_str());
    }
  }

  for (list<GlobalVarDeclaration*>::iterator var_it = m_variables.begin(); var_it!=m_variables.end(); var_it++) {
    GlobalVarDeclaration* var = *var_it;
    if (!symtab.insertVariable(var)) {
      errmsg("Duplicate global variable %s",var->m_name->c_str());
    }
  }

  for (list<FuncDefinition*>::iterator func_it = m_functions.begin(); func_it!=m_functions.end(); func_it++) {
    FuncDefinition* func = *func_it;
    func->resolve(&symtab);
  }
}

void Program::check_types()
{
  for (list<FuncDefinition*>::iterator func_it = m_functions.begin(); func_it!=m_functions.end(); func_it++) {
    FuncDefinition* func = *func_it;
    func->check_types();
  }
}

void FuncDefinition::resolve(SymbolTable* symtab)
{
  symtab->enterScope();

  symtab->insertVariable(m_return_value);

  for (list<ParamDeclaration*>::iterator it=m_params->begin(); it!=m_params->end(); it++) {
    ParamDeclaration* param = *it;
    symtab->insertVariable(param);
  }

  m_stmts->resolve(symtab);

  symtab->leaveScope();
}

void Block::resolve(SymbolTable* symtab)
{
  symtab->enterScope();

  for (list<LocalVarDeclaration*>::iterator it=m_declarations.begin(); it!=m_declarations.end(); it++) {
    LocalVarDeclaration* decl = *it;
    symtab->insertVariable(decl);
  }

  for (list<Stmt*>::iterator it=m_stmts.begin(); it!=m_stmts.end(); it++) {
    Stmt* stmt = *it;
    stmt->resolve(symtab);
  }

  symtab->leaveScope();
}

void Block::check_types()
{
  for (list<Stmt*>::iterator it=m_stmts.begin(); it!=m_stmts.end(); it++) {
    Stmt* stmt = *it;
    stmt->check_types();
  }
}

void AssignmentStmt::check_types()
{
  m_lvalue->check_types();
  m_expr->check_types();
}

void IfStmt::check_types()
{
  m_expr->check_types();
  m_true->check_types();
  m_false->check_types();
}

void WhileStmt::check_types()
{
  m_expr->check_types();
  m_stmts->check_types();
}

void FunctionCall::resolve(SymbolTable* symtab)
{
  m_definition = symtab->lookupFunction(m_name);
  if (m_definition==NULL) {
    errmsg("Call of undefined function %s",m_name->c_str());
  }

  for (list<Expr*>::iterator it=m_args->begin(); it!=m_args->end(); it++) {
    Expr* expr = *it;
    expr->resolve(symtab);
  }
}

void FunctionCall::check_types()
{
  m_type = m_definition->m_type;
  for (list<Expr*>::iterator it=m_args->begin(); it!=m_args->end(); it++) {
    Expr* expr = *it;
    expr->check_types();
  }

  if (m_args->size()==m_definition->m_params->size()) {
    list<ParamDeclaration*>::iterator it_p = m_definition->m_params->begin();
    list<Expr*>::iterator it_a = m_args->begin();
    while (it_a!=m_args->end()) {
      it_p++;
      it_a++;
    }
  }
  else {
    errmsg("Incompatible number of arguments");
  }
}

void VarAccess::resolve(SymbolTable* symtab)
{
  m_decl = symtab->lookupVariable(m_name);
  if (m_decl==NULL) {
    errmsg("Undefined variable %s",m_name->c_str());
  }
}

void ArithmeticExpr::calculate_types()
{
  /* TODO */
}

void NegExpr::calculate_types()
{
  /* TODO */
}
