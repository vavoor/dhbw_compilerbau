#ifndef HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D
#define HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D

#include <string>
#include <list>
#include <map>
using namespace std;

class Node
{
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

class UndimensionedArrayType : public Type
{
public:
  Type* m_base;

  UndimensionedArrayType(Type* base)
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
  string m_name;
  Type* m_type;

  VarDeclaration(string& name, Type* type)
  : m_name(name), m_type(type)
  {}

};

class GlobalVarDeclaration : public VarDeclaration
{
public:
  GlobalVarDeclaration(string& name, Type* type)
  : VarDeclaration(name,type)
  {}
};

class ParamDeclaration : public VarDeclaration
{
public:
  ParamDeclaration(string& name, Type* type)
  : VarDeclaration(name,type)
  {}
};

class LocalVarDeclaration : public VarDeclaration
{
public:
  LocalVarDeclaration(string& name, Type* type)
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
  string m_name;
  VarDeclaration* m_decl;
};

class ArrayAccess : public LValue
{
public:
  LValue* m_base;
  Expression* m_index;

  ArrayAccess(LValue *base, Expression *expr)
  : m_base(base), m_index(expr)
  {}
};

class Expression : public Node
{
};

class BinaryExpr : public Expression
{
public:
  Expression* m_left;
  expression* m_right;

  BinaryExpr(Expression* l, Expression* r)
  : m_left(l), m_right(r)
  {}
};

class UnaryExpr : public Expression
{
public:
  Expression* m_child;

  UnaryExpr(Expression* child)
  : m_child(child)
  {}
};

class AndExpr : public BinaryExpr
{
public:
  AndExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class OrExpr : public BinaryExpr
{
public:
  OrExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class LessExpr : public BinaryExpr
{
public:
  LessExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class LessEqualExpr : public BinaryExpr
{
public:
  LessEqualExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class GreaterExpr : public BinaryExpr
{
public:
  GreaterExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class GreaterEqualExpr : public BinaryExpr
{
public:
  GreaterEqualExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class EqualExpr : public BinaryExpr
{
public:
  EqualExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class NotEqualExpr : public BinaryExpr
{
public:
  NotEqualExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class AddExpr : public BinaryExpr
{
public:
  AddExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class SubExpr : public BinaryExpr
{
public:
  SubExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class MultExpr : public BinaryExpr
{
public:
  MultExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class DivExpr : public BinaryExpr
{
public:
  DivExpr(Expression* l, Expression* r)
  : BinaryExpr(l,r)
  {}
};

class NotExpr : public UnaryExpr
{
public:
  NotExpr(Expression* child)
  : UnaryExpr(child)
  {}
};

class NegExpr : public UnaryExpr
{
public:
  NegExpr(Expression* child)
  : UnaryExpr(child)
  {}
};

class StringLiteral : public Expression
{
public:
  string m_value;

  StringLiteral(string& val)
  : m_value(val)
  {}
};

class IntLiteral : public Expression
{
public:
  int m_value;

  IntLiteral(int val)
  : m_value(val)
  {}
};

class FloatLiteral : public Expression
{
public:
  double m_value;

  FloatLiteral(double val)
  : m_value(val)
  {}
};

class FunctionCall : public Expression
{
public:
  string m_name;
  List<Expression*>* m_args;

  FunctionCall(string& name, List<Expression*>* args)
  : m_name(name), m_args(args)
  {}
};

class VariableExpr : public Expression
{
public:
  LValue* m_lvalue;

  VariableExpr(LValue *lval)
  : m_lvalue(lval)
  {}
};

class Statement : public Node
{
};

class Block : public Node
{
public:
  List<Statement*>* m_stmts;
  List<LocalVarDeclaration*>* m_declarations;

};

class FuncDeclaration : public Node
{
public:
  string m_name;
  List<ParamDeclaration*>* m_params;
  Block* m_stmts;

  FuncDeclaration(string& name, List<ParamDeclaration*>* params, Block* block)
  : m_name(name), m_params(params), m_stmts(block)
  {}
};

class AssignmentStmt : public Statement
{
public:
  LValue* m_lvalue;
  Expression* m_expr;

  AssignmentStmt(LValue* l, Expression* expr)
  : m_lvalue(l), m_expr(expr)
  {}
};

class IfStmt : public Statement
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

#endif
