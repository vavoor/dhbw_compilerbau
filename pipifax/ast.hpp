#ifndef HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D
#define HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D

#include <string>
#include <list>
#include <map>
using namespace std;

#include "symtab.hpp"

class Expr;
class FuncDefinition;
class IntType;
class StringType;
class FloatType;
class VoidType;
class ArrayType;
class ReferenceType;
class DimensionlessArrayType;

/* Superclass for all nodes in the AST */
class Node
{
public:
  virtual void resolve(SymbolTable* symtab) {}
  virtual void check_types() {}
};

/* Superclass of all nodes representing types */
class Type : public Node
{
public:
  /* Returns the type of a value of this type. This is to solve the problem that the
     value type of a reference is not a reference but the referenced type 0*/
  virtual Type* value_type() {
    return this;
  }

  /* a op b : b->is_compatible(a) */
  /* a = b  : b->is_compatible(a) */
  virtual bool is_compatible(Type* t) = 0;
  virtual bool is_compatible_with(Type* t) { return false; }
};


/* There is one object instance() that represents the int type */
class IntType : public Type
{
public:
  static IntType* instance() {
    return &s_instance;
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

protected:
  virtual bool is_compatible_with(IntType* t) {
    return true;
  }

private:
  static IntType s_instance;
  IntType() {}
};

/* There is one object instance() that represents the float type */
class FloatType : public Type
{
public:
  static FloatType* instance() {
    return &s_instance;
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

protected:
  virtual bool is_compatible_with(FloatType* t) {
    return true;
  }

private:
  static FloatType s_instance;
  FloatType() {}
};

/* There is one object instance() that represents the string type */
class StringType : public Type
{
public:
  static StringType* instance() {
    return &s_instance;
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

protected:
  virtual bool is_compatible_with(StringType* t) {
    return true;
  }

private:
  static StringType s_instance;
  StringType() {}
};

/* This object represents a functio with no return type. There is only one object instance() */
class VoidType : public Type
{
public:
  static VoidType* instance() {
    return &s_instance;
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

private:
  static VoidType s_instance;
  VoidType() {}
};

/* A node of this type represents an array type
   for a [17] string, m_base is the string type and m_dim is 17 */
class ArrayType : public Type
{
public:
  Type* m_base;
  int m_dim;

  ArrayType(Type* base, int dimension)
  : m_base(base), m_dim(dimension)
  {}

  virtual Type* value_type() {
    return m_base;
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

protected:
  virtual bool is_compatible_with(ArrayType* t) {
    return m_dim==t->m_dim && t->m_base->is_compatible(m_base);
  }
};

/* A node of this type represents an array without a dimension
   for x *[] int, m_base is the int type */
class DimensionlessArrayType : public Type
{
public:
  Type* m_base;

  DimensionlessArrayType(Type* base)
  : m_base(base)
  {}

  virtual Type* value_type() {
    return m_base;
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

protected:
  virtual bool is_compatible_with(DimensionlessArrayType* t) {
    return t->m_base->is_compatible_with(m_base);
  }
  virtual bool is_compatible_with(ArrayType* t) {
    return t->m_base->is_compatible_with(m_base);
  }
};

/* A node of this type represents a reference to another type
   for x* string, m_base is the string type */
class ReferenceType : public Type
{
public:
  Type* m_base;

  ReferenceType(Type* base)
  : m_base(base)
  {}

  virtual Type* value_type() {
    return m_base;
  }

  virtual bool is_compatible(Type* t) { return is_compatible_with(this); }
  
protected:
  virtual bool is_compatible_with(Type* t) {
    return t->is_compatible(m_base); 
  }
};

/* Superclass representing all kinds of variable declaration */
class VarDeclaration : public Node
{
public:
  string* m_name;
  Type* m_type;

  VarDeclaration(string* name, Type* type)
  : m_name(name), m_type(type)
  {}

};

/* Represents the declaration of a global variable */
class GlobalVarDeclaration : public VarDeclaration
{
public:
  GlobalVarDeclaration(string* name, Type* type)
  : VarDeclaration(name,type)
  {}
};

/* Represents a parameter in a function definition */
class ParamDeclaration : public VarDeclaration
{
public:
  ParamDeclaration(string*name, Type* type)
  : VarDeclaration(name,type)
  {}
};

/* Represents a declaration of a local variable in a code block */
class LocalVarDeclaration : public VarDeclaration
{
public:
  LocalVarDeclaration(string* name, Type* type)
  : VarDeclaration(name,type)
  {}
};

/* Superclass of all Expressions */
class Expr : public Node
{
public:
  Type* m_type;
};

/* Superclass of variables, that can occur on the left side of an assignment, i.e.
   they are assignable */
class LValue : public Node
{
public:
  Type* m_type;
};

/* Represents a variable name, e.g. a = ... or a[...]= ... */
class VarAccess : public LValue
{
public:
  string* m_name;
  VarDeclaration* m_decl;

  VarAccess(string* name)
  : m_name(name), m_decl(NULL)
  {}

  virtual void resolve(SymbolTable* symtab);
  virtual void check_types() {
    m_type = m_decl->m_type->value_type();
  }
};

/* Represents an array access, e.g. for a[5] m_base is a and m_index is 5 */
class ArrayAccess : public LValue
{
public:
  LValue* m_base;
  Expr* m_index;

  ArrayAccess(LValue *base, Expr *expr)
  : m_base(base), m_index(expr)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_base->resolve(symtab);
    m_index->resolve(symtab);
  }

  virtual void check_types() {
    m_base->check_types();
    m_type = m_base->m_type->value_type();
  }
};

/* Superclass of all Expressions with two children */
class BinaryExpr : public Expr
{
public:
  Expr* m_left;
  Expr* m_right;

  BinaryExpr(Expr* l, Expr* r)
  : m_left(l), m_right(r)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_left->resolve(symtab);
    m_right->resolve(symtab);
  }

  /* Valid for most subtypes ... */
  virtual void calculate_types() {
    m_left->check_types();
    m_right->check_types();
    /* TODO : set m_type! */
  }
};

/* Superclass of all expressions with one child */
class UnaryExpr : public Expr
{
public:
  Expr* m_child;

  UnaryExpr(Expr* child)
  : m_child(child)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_child->resolve(symtab);
  }
  
  virtual void check_types() {
    m_child->check_types();
    /* TODO : set m_type! */
  }
};

class AndExpr : public BinaryExpr
{
public:
  AndExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class OrExpr : public BinaryExpr
{
public:
  OrExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class CompExpr : public BinaryExpr
{
public:
  CompExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class LessExpr : public CompExpr
{
public:
  LessExpr(Expr* l, Expr* r)
  : CompExpr(l,r)
  {}
};

class LessEqualExpr : public CompExpr
{
public:
  LessEqualExpr(Expr* l, Expr* r)
  : CompExpr(l,r)
  {}
};

class GreaterExpr : public CompExpr
{
public:
  GreaterExpr(Expr* l, Expr* r)
  : CompExpr(l,r)
  {}
};

class GreaterEqualExpr : public CompExpr
{
public:
  GreaterEqualExpr(Expr* l, Expr* r)
  : CompExpr(l,r)
  {}
};

class EqualExpr : public CompExpr
{
public:
  EqualExpr(Expr* l, Expr* r)
  : CompExpr(l,r)
  {}
};

class NotEqualExpr : public CompExpr
{
public:
  NotEqualExpr(Expr* l, Expr* r)
  : CompExpr(l,r)
  {}
};

class ArithmeticExpr : public BinaryExpr
{
public:
  ArithmeticExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}

  virtual void calculate_types();
};

class AddExpr : public ArithmeticExpr
{
public:
  AddExpr(Expr* l, Expr* r)
  : ArithmeticExpr(l,r)
  {}
};

class SubExpr : public ArithmeticExpr
{
public:
  SubExpr(Expr* l, Expr* r)
  : ArithmeticExpr(l,r)
  {}
};

class MultExpr : public ArithmeticExpr
{
public:
  MultExpr(Expr* l, Expr* r)
  : ArithmeticExpr(l,r)
  {}
};

class DivExpr : public ArithmeticExpr
{
public:
  DivExpr(Expr* l, Expr* r)
  : ArithmeticExpr(l,r)
  {}
};

class NotExpr : public UnaryExpr
{
public:
  NotExpr(Expr* child)
  : UnaryExpr(child)
  {}

  virtual void calculate_types() {
    m_type = IntType::instance();
  }
};

class NegExpr : public UnaryExpr
{
public:
  NegExpr(Expr* child)
  : UnaryExpr(child)
  {}

  virtual void calculate_types();
};

class IntConversion : public UnaryExpr
{
public:
  IntConversion(Expr* expr)
  : UnaryExpr(expr)
  {}

  virtual void check_types() {
    UnaryExpr::check_types();
    m_type = IntType::instance();
  }
};

class FloatConversion : public UnaryExpr
{
public:
  FloatConversion(Expr* expr)
  : UnaryExpr(expr)
  {}

  virtual void check_types() {
    UnaryExpr::check_types();
    m_type = FloatType::instance();
  }
};

/* Represents a string literal, such as "Hello Wordl!" */
class StringLiteral : public Expr
{
public:
  string* m_value;

  StringLiteral(char *val) {
    m_value = new string(val);
  }

  virtual void check_types() {
    m_type = StringType::instance();
  }
};

/* Represents an integer literal, such as 4711 */
class IntLiteral : public Expr
{
public:
  int m_value;

  IntLiteral(int val)
  : m_value(val)
  {}

  virtual void check_types() {
    m_type = IntType::instance();
  }
};

/* Represents a float literal, such as 3.1415926 */
class FloatLiteral : public Expr
{
public:
  double m_value;

  FloatLiteral(double val)
  : m_value(val)
  {}

  virtual void calculate_types() {
    m_type = FloatType::instance();
  }
};

/* Represents a call to a function, e.g. sqrt(16) */
class FunctionCall : public Expr
{
public:
  string* m_name;
  list<Expr*>* m_args;
  FuncDefinition* m_definition;

  FunctionCall(string*name, list<Expr*>* args)
  : m_name(name), m_args(args), m_definition(NULL)
  {}

  virtual void resolve(SymbolTable* symtab);
  virtual void check_types();
};

/* Represents an access to a variable, e.g. a or a[34]. This is a wrapper around a
   VariableAccess or ArrayAccess with translates the address into a value */
class VariableExpr : public Expr
{
public:
  LValue* m_lvalue;

  VariableExpr(LValue *lval)
  : m_lvalue(lval)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_lvalue->resolve(symtab);
  }

  virtual void check_types() {
    m_lvalue->check_types();
    m_type = m_lvalue->m_type;
  }
};

/* Superclass for all statements */
class Stmt : public Node
{
};

/* Represents a sequence of variable declarations and statements in curly brackets */
class Block : public Node
{
public:
  list<Stmt*> m_stmts;
  list<LocalVarDeclaration*> m_declarations;

  virtual void resolve(SymbolTable* symtab);
  virtual void check_types();
};

/* Represents a function definition like func f(a int) float {...} */
class FuncDefinition : public Node
{
public:
  string* m_name;
  list<ParamDeclaration*>* m_params;
  Type* m_type;
  ParamDeclaration* m_return_value;
  Block* m_stmts;

  FuncDefinition(string* name, list<ParamDeclaration*>* params, Type* type, Block* block)
  : m_name(name), m_params(params), m_type(type), m_stmts(block)
  {
    m_return_value = new ParamDeclaration(m_name,m_type);
  }

  virtual void resolve(SymbolTable* symtab);

  virtual void check_types() {
    m_stmts->check_types();
  }
};

/* Represents an assignment with an lvalue and an expression */
class AssignmentStmt : public Stmt
{
public:
  LValue* m_lvalue;
  Expr* m_expr;

  AssignmentStmt(LValue* l, Expr* expr)
  : m_lvalue(l), m_expr(expr)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_lvalue->resolve(symtab);
    m_expr->resolve(symtab);
  }

  virtual void check_types();
};

class IfStmt : public Stmt
{
public:
  Expr* m_expr;
  Block* m_true;
  Block* m_false; /* block with no statements if not present  */

  IfStmt(Expr* expr, Block* t, Block* f)
  : m_expr(expr), m_true(t), m_false(f)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_expr->resolve(symtab);
    m_true->resolve(symtab);
    m_false->resolve(symtab);
  }

  virtual void check_types();
};

class WhileStmt : public Stmt
{
public:
  Expr* m_expr;
  Block* m_stmts;

  WhileStmt(Expr* expr, Block* stmts)
  : m_expr(expr), m_stmts(stmts)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_expr->resolve(symtab);
    m_stmts->resolve(symtab);
  }

  virtual void check_types();
};

/* Represents a function call as a statement, i.e. it wraps a function call expression */
class FunctionCallStmt : public Stmt
{
public:
  FunctionCall* m_function;

  FunctionCallStmt(FunctionCall* function)
  : m_function(function)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_function->resolve(symtab);
  }

  virtual void check_types() {
    m_function->check_types();
  }
};

/* Represents the overall program. It's the root of the AST */
class Program
{
public:
  list<GlobalVarDeclaration*> m_variables;
  list<FuncDefinition*> m_functions;

  void resolve();
  void check_types();
};

extern Program* the_program;

#endif
