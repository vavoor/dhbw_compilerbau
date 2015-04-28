#ifndef HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D
#define HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D

#include <string>
#include <list>
#include <map>
using namespace std;

#include "symtab.hpp"

class Expr;

/* Superclass for all nodes in the AST */
class Node
{
public:
  virtual void resolve(SymbolTable* symtab) {};
};

/* Superclass of all nodes representing types */
class Type : public Node
{
};


/* There is one object (getInstance) that represents the int type */
class IntType : public Type
{
public:
  static IntType* getInstance() {
    if (m_singleton==NULL) {
      m_singleton = new IntType;
    }
    return m_singleton;
  }

private:
  static IntType* m_singleton;
  IntType() {}
};

/* There is one object (getInstance) that represents the float type */
class FloatType : public Type
{
public:
  static FloatType* getInstance() {
    if (m_singleton==NULL) {
      m_singleton = new FloatType;
    }
    return m_singleton;
  }

private:
  static FloatType* m_singleton;
  FloatType() {}
};

/* There is one object (getInstance) that represents the string type */
class StringType : public Type
{
public:
  static StringType* getInstance() {
    if (m_singleton==NULL) {
      m_singleton = new StringType;
    }
    return m_singleton;
  }

private:
  static StringType* m_singleton;
  StringType() {}
};

/* This object represents a functio with no return type. There is only one object (getInstance) */
class VoidType : public Type
{
public:
  static VoidType* getInstance() {
    if (m_singleton==NULL) {
      m_singleton = new VoidType;
    }
    return m_singleton;
  }

private:
  static VoidType* m_singleton;
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

/* Superclass of variables, that can occur on the left side of an assignment, i.e.
   they are assignable */
class LValue : public Node
{
};

/* Represents a variable name, e.g. a = ... or a[...]= ... */
class VarAccess : public LValue
{
public:
  string* m_name;

  VarAccess(string* name)
  : m_name(name)
  {}
};

/* Represents an array access, e.g. a[5] */
class ArrayAccess : public LValue
{
public:
  LValue* m_base;
  Expr* m_index;

  ArrayAccess(LValue *base, Expr *expr)
  : m_base(base), m_index(expr)
  {}
};

/* Superclass of all Expressions */
class Expr : public Node
{
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
};

/* Superclass of all expressions with one child */
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

class IntConversion : public UnaryExpr
{
public:
  IntConversion(Expr* child)
  : UnaryExpr(expr)
  {}
};

class FloatConversion : public UnaryExpr
{
public:
  FloatConversion(Expr* expr)
  : UnaryExpr(expr)
  {}
};

/* Represents a string literal, such as "Hello Wordl!" */
class StringLiteral : public Expr
{
public:
  string* m_value;

  StringLiteral(char *val) {
    m_value = new string(val);
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
};

/* Represents a float literal, such as 3.1415926 */
class FloatLiteral : public Expr
{
public:
  double m_value;

  FloatLiteral(double val)
  : m_value(val)
  {}
};

/* Represents a call to a function, e.g. sqrt(16) */
class FunctionCall : public Expr
{
public:
  string* m_name;
  list<Expr*>* m_args;

  FunctionCall(string*name, list<Expr*>* args)
  : m_name(name), m_args(args)
  {}
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
};

/* Represents a function definition like func f(a int) float {...} */
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

/* Represents an assignment with an lvalue and an expression */
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
  Block* m_false; /* block with no statements if not present  */

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

/* Represents a function call as a statement, i.e. it wraps a function call expression */
class FunctionCallStmt : public Stmt
{
public:
  FunctionCall* m_function;

  FunctionCallStmt(FunctionCall* function)
  : m_function(function)
  {}
};

/* Represents the overall program. It's the root of the AST */
class Program
{
public:
  list<GlobalVarDeclaration*> m_variables;
  list<FuncDefinition*> m_functions;

  void resolve();
};

extern Program* the_program;

#endif
