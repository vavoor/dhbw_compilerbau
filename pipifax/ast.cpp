#include "ast.hpp"
#include "err.hpp"
#include "symtab.hpp"

Program* the_program;

IntType IntType::s_instance;
FloatType FloatType::s_instance;
StringType StringType::s_instance;
VoidType VoidType::s_instance;
ErrorType ErrorType::s_instance;


ArrayType::ArrayType(int line, Type* base, int dimension)
: m_base(base), m_dim(dimension)
{
  m_location = line;
  if (m_dim<=0) {
    errmsg(m_location,"Array dimension must be greater than 0");
  }
}

void Program::resolve()
{
  SymbolTable symtab;

  for (list<FuncDefinition*>::iterator func_it = m_functions.begin(); func_it!=m_functions.end(); func_it++) {
    FuncDefinition* func = *func_it;
    if (!symtab.insertFunction(func)) {
      errmsg(func->m_location,"Duplicate function %s",func->m_name->c_str());
    }
  }

  for (list<GlobalVarDeclaration*>::iterator var_it = m_variables.begin(); var_it!=m_variables.end(); var_it++) {
    GlobalVarDeclaration* var = *var_it;
    if (!symtab.insertVariable(var)) {
      errmsg(var->m_location,"Duplicate global variable %s",var->m_name->c_str());
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

void Program::prepare()
{
  GenerationContext context;

  context.reset_offset();
  for (list<GlobalVarDeclaration*>::iterator var_it = m_variables.begin(); var_it!=m_variables.end(); var_it++) {
    GlobalVarDeclaration* var = *var_it;
    var->prepare(&context);
  }

  context.reset_offset();
  for (list<FuncDefinition*>::iterator func_it = m_functions.begin(); func_it!=m_functions.end(); func_it++) {
    FuncDefinition* func = *func_it;
    func->prepare(&context);
  }
}

void Program::generate(ostream& os)
{
  GenerationContext context;

  os << ".CODE" <<endl;
  for (list<FuncDefinition*>::iterator func_it = m_functions.begin(); func_it!=m_functions.end(); func_it++) {
    FuncDefinition* func = *func_it;
    func->generate(&context,os);
  }

  os << ".DATA" << endl;
  for (list<GlobalVarDeclaration*>::iterator var_it = m_variables.begin(); var_it!=m_variables.end(); var_it++) {
    GlobalVarDeclaration* var = *var_it;
    var->generate(&context,os);
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

void FuncDefinition::prepare(GenerationContext* ct)
{
  int offset = 0;
  for (list<ParamDeclaration*>::reverse_iterator rit=m_params->rbegin(); rit!=m_params->rend(); ++rit) {
    ParamDeclaration* p = *rit;
    p->m_offset = offset+8;
    offset += p->m_type->size();
  }
  m_return_value->m_offset = offset+8;
  offset += m_return_value->m_type->size();
  m_arg_size = offset;

  ct->reset_offset();
  m_stmts->prepare(ct);
}

void FuncDefinition::generate(GenerationContext* ct, ostream& os)
{
}

void GlobalVarDeclaration::prepare(GenerationContext* ct)
{
  m_offset = ct->offset();
  ct->increase(m_type->size());
}


void GlobalVarDeclaration::generate(GenerationContext* ct, ostream& os)
{
  os << *m_name << ": DS(" << m_type->size() <<")" << endl;
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

void Block::prepare(GenerationContext* ct)
{
  int offset = ct->offset();
  for (list<LocalVarDeclaration*>::iterator it=m_declarations.begin(); it!=m_declarations.end(); ++it) {
    LocalVarDeclaration* v = *it;
    ct->increase(v->m_type->size());
    v->m_offset = -ct->offset();
  }
  m_var_size = ct->offset()-offset;
  for (list<Stmt*>::iterator it=m_stmts.begin(); it!=m_stmts.end(); ++it) {
    (*it)->prepare(ct);
  }
  ct->reset_offset(offset);
}

void AssignmentStmt::check_types()
{
  m_lvalue->check_types();
  m_expr->check_types();

  if (!m_expr->m_type->is_compatible(m_lvalue->m_type)) {
    errmsg(m_lvalue->m_location,"Type mismatch in assignment");
  }
}

void IfStmt::check_types()
{
  m_expr->check_types();
  m_true->check_types();
  m_false->check_types();

  if (!m_expr->m_type->is_compatible(IntType::instance())) {
    errmsg(m_expr->m_location,"Expression of condition must be integer");
  }
}

void WhileStmt::check_types()
{
  m_expr->check_types();
  m_stmts->check_types();
  if (!m_expr->m_type->is_compatible(IntType::instance())) {
    errmsg(m_expr->m_location,"Expression in condition must be integer");
  }
}

void FunctionCall::resolve(SymbolTable* symtab)
{
  m_definition = symtab->lookupFunction(m_name);
  if (m_definition==NULL) {
    errmsg(m_location,"Call of undefined function %s",m_name->c_str());
  }

  for (list<Expr*>::iterator it=m_args->begin(); it!=m_args->end(); it++) {
    Expr* expr = *it;
    expr->resolve(symtab);
  }
}

void FunctionCall::check_types()
{
  for (list<Expr*>::iterator it=m_args->begin(); it!=m_args->end(); it++) {
    Expr* expr = *it;
    expr->check_types();
  }

  if (m_args->size()==m_definition->m_params->size()) {
    int para_no = 1;
    list<ParamDeclaration*>::iterator it_p = m_definition->m_params->begin();
    list<Expr*>::iterator it_a = m_args->begin();
    while (it_a!=m_args->end()) {
      ParamDeclaration* p = *it_p;
      Expr* e = *it_a;
      if (!e->m_type->is_compatible(p->m_type)) {
        errmsg(e->m_location,"Illegal type of parameter %d in function call",para_no);
      }
      it_p++;
      it_a++;
      para_no++;
    }
  }
  else {
    errmsg(m_location,"Incompatible number of arguments");
  }

  m_type = m_definition->m_type;
}

void VarAccess::resolve(SymbolTable* symtab)
{
  m_decl = symtab->lookupVariable(m_name);
  if (m_decl==NULL) {
    errmsg(m_location,"Undefined variable %s",m_name->c_str());
  }
}

void ArrayAccess::check_types()
{
    m_base->check_types();
    m_index->check_types();

    if (!m_index->m_type->is_compatible(IntType::instance())) {
      errmsg(m_index->m_location,"Array index type must be integer");
    }
    else if (!m_base->m_type->is_indexed()) {
      errmsg(m_location,"Indexed access to non-array type");
    }
    else {
      m_type = m_base->m_type->element_type();
    }
}

void LogicalExpr::check_type()
{
  BinaryExpr::check_types();
  if (!m_left->m_type->is_compatible(IntType::instance())) {
    errmsg(m_left->m_location,"Operands of logical expression must be integer");
  }
  else if (!m_right->m_type->is_compatible(IntType::instance())) {
    errmsg(m_right->m_location,"Operands of logical expression must be integer");
  }
  else {
    m_type = IntType::instance();
  }
}

void CompExpr::check_types()
{
  BinaryExpr::check_types();

  bool is_legal_type =
    m_left->m_type->is_compatible(IntType::instance()) ||
    m_left->m_type->is_compatible(FloatType::instance()) ||
    m_left->m_type->is_compatible(StringType::instance()) ||
    m_right->m_type->is_compatible(IntType::instance()) ||
    m_right->m_type->is_compatible(FloatType::instance()) ||
    m_right->m_type->is_compatible(StringType::instance());

  if (!is_legal_type) {
    errmsg(m_left->m_location,"Type in comparison must be int, float or string");
  }
  else if (!m_right->m_type->is_compatible(m_left->m_type)) {
    errmsg(m_left->m_location,"Types in comparison don't match");
  }
  else {
    m_type = IntType::instance();
  }
}

void ArithmeticExpr::check_types()
{
  BinaryExpr::check_types();

  bool is_legal_type =
    m_left->m_type->is_compatible(IntType::instance()) ||
    m_left->m_type->is_compatible(FloatType::instance()) ||
    m_right->m_type->is_compatible(IntType::instance()) ||
    m_right->m_type->is_compatible(FloatType::instance());

  if (!is_legal_type) {
    errmsg(m_left->m_location,"Type in arithmetic operation must be int or float");
  }
  else if (!m_right->m_type->is_compatible(m_left->m_type)) {
    errmsg(m_left->m_location,"Types in arithmetic operation don't match");
  }
  else {
    m_type = m_left->m_type;
  }
}

void NotExpr::check_types()
{
  UnaryExpr::check_types();

  if (!m_child->m_type->is_compatible(IntType::instance())) {
    errmsg(m_child->m_location,"Operand of ! must be integer");
  }
  else {
    m_type = IntType::instance();
  }
}

void NegExpr::check_types()
{
  UnaryExpr::check_types();

  int is_legal_type = m_child->m_type->is_compatible(IntType::instance()) || m_child->m_type->is_compatible(FloatType::instance());
  if (!is_legal_type) {
    errmsg(m_child->m_location,"Operand must be integer or float");
  }
  else {
    m_type = m_child->m_type;
  }
}

void IntConversion::check_types()
{
  UnaryExpr::check_types();

  if (m_child->m_type->is_compatible(FloatType::instance())) {
    errmsg(m_child->m_location,"Operand of int() must be float");
  }
  else {
    m_type = IntType::instance();
  }
}

void FloatConversion::check_types()
{
  UnaryExpr::check_types();

  if (!m_child->m_type->is_compatible(IntType::instance())) {
    errmsg(m_child->m_location,"Operand of float() must be int");
  }
  else {
    m_type = FloatType::instance();
  }
}

Type* Type::element_type()
{
  return VoidType::instance();
}
