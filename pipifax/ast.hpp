#ifndef HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D
#define HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D

#include <string>
#include <list>
#include <map>
using namespace std;

#include "symtab.hpp"

class Expr;

class Node
{
public:
  virtual void resolve(SymbolTable* symtab) {};
};

class Type : public Node
{
};

class IntType : public Type
{
};

class FloatType : public Type
{
};

class StringType : public Type
{
};

class VoidType : public Type
{
};

class ArrayType : public Type
{
public:
  Type* m_base;
  int m_dim;

  ArrayType(Type* base, int dimension)
  : m_base(base), m_dim(dimension)
  {}
};

/* x *[] int */
class ArrayRefType : public Type
{
public:
  Type* m_base;

  ArrayRefType(Type* base)
  : m_base(base)
  {}
};

class ReferenceType : public Type
{
public:
  Type* m_base;

  ReferenceType(Type* base)
  : m_base(base)
  {}
};

class VarDeclaration : public Node
{
public:
  string* m_name;
  Type* m_type;

  VarDeclaration(string* name, Type* type)
  : m_name(name), m_type(type)
  {}

};

class GlobalVarDeclaration : public VarDeclaration
{
public:
  GlobalVarDeclaration(string* name, Type* type)
  : VarDeclaration(name,type)
  {}
};

class ParamDeclaration : public VarDeclaration
{
public:
  ParamDeclaration(string*name, Type* type)
  : VarDeclaration(name,type)
  {}
};

class LocalVarDeclaration : public VarDeclaration
{
public:
  LocalVarDeclaration(string* name, Type* type)
  : VarDeclaration(name,type)
  {}
};

/* Kann auf der linken Seite einer Zuweisung vorkommen */
class LValue : public Node
{
};

class VarAccess : public LValue
{
public:
  string* m_name;
  
  VarAccess(string* name)
  : m_name(name)
  {}
};

class ArrayAccess : public LValue
{
public:
  LValue* m_base;
  Expr* m_index;

  ArrayAccess(LValue *base, Expr *expr)
  : m_base(base), m_index(expr)
  {}
};

class Expr : public Node
{
};

class BinaryExpr : public Expr
{
public:
  Expr* m_left;
  Expr* m_right;

  BinaryExpr(Expr* l, Expr* r)
  : m_left(l), m_right(r)
  {}
};

class UnaryExpr : public Expr
{
public:
  Expr* m_child;

  UnaryExpr(Expr* child)
  : m_child(child)
  {}
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

class LessExpr : public BinaryExpr
{
public:
  LessExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class LessEqualExpr : public BinaryExpr
{
public:
  LessEqualExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class GreaterExpr : public BinaryExpr
{
public:
  GreaterExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class GreaterEqualExpr : public BinaryExpr
{
public:
  GreaterEqualExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class EqualExpr : public BinaryExpr
{
public:
  EqualExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class NotEqualExpr : public BinaryExpr
{
public:
  NotEqualExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class AddExpr : public BinaryExpr
{
public:
  AddExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class SubExpr : public BinaryExpr
{
public:
  SubExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class MultExpr : public BinaryExpr
{
public:
  MultExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class DivExpr : public BinaryExpr
{
public:
  DivExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}
};

class NotExpr : public UnaryExpr
{
public:
  NotExpr(Expr* child)
  : UnaryExpr(child)
  {}
};

class NegExpr : public UnaryExpr
{
public:
  NegExpr(Expr* child)
  : UnaryExpr(child)
  {}
};

class StringLiteral : public Expr
{
public:
  string* m_value;

  StringLiteral(char *val) {
    m_value = new string(val);
  }
};

class IntLiteral : public Expr
{
public:
  int m_value;

  IntLiteral(int val)
  : m_value(val)
  {}
};

class FloatLiteral : public Expr
{
public:
  double m_value;

  FloatLiteral(double val)
  : m_value(val)
  {}
};

class FunctionCall : public Expr
{
public:
  string* m_name;
  list<Expr*>* m_args;

  FunctionCall(string*name, list<Expr*>* args)
  : m_name(name), m_args(args)
  {}
};

class VariableExpr : public Expr
{
public:
  LValue* m_lvalue;

  VariableExpr(LValue *lval)
  : m_lvalue(lval)
  {}
};

class Stmt : public Node
{
};

class Block : public Node
{
public:
  list<Stmt*> m_stmts;
  list<LocalVarDeclaration*> m_declarations;
};

class FuncDefinition : public Node
{
public:
  string* m_name;
  list<ParamDeclaration*>* m_params;
  Type* m_type;
  Block* m_stmts;

  FuncDefinition(string* name, list<ParamDeclaration*>* params, Type* type, Block* block)
  : m_name(name), m_params(params), m_type(type), m_stmts(block)
  {}
};

class AssignmentStmt : public Stmt
{
public:
  LValue* m_lvalue;
  Expr* m_expr;

  AssignmentStmt(LValue* l, Expr* expr)
  : m_lvalue(l), m_expr(expr)
  {}
};

class IfStmt : public Stmt
{
public:
  Expr* m_expr;
  Block* m_true;
  Block* m_false; /* kann NULL sein
  */

  IfStmt(Expr* expr, Block* t, Block* f)
  : m_expr(expr), m_true(t), m_false(f)
  {}
};

class WhileStmt : public Stmt
{
public:
  Expr* m_expr;
  Block* m_stmts;

  WhileStmt(Expr* expr, Block* stmts)
  : m_expr(expr), m_stmts(stmts)
  {}
};

class FunctionCallStmt : public Stmt
{
public:
  FunctionCall* m_function;

  FunctionCallStmt(FunctionCall* function)
  : m_function(function)
  {}
};

class Program
{
public:
  list<GlobalVarDeclaration*> m_variables;
  list<FuncDefinition*> m_functions;
  
  void resolve();
};

extern Program* the_program;

#endif
