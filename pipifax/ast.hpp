#ifndef HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D
#define HA15F17E1_3F8B_41A3_BB6A_C8757DD7985D

#include <boost/lexical_cast.hpp>

#include <iostream>
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
class ErrorType;

class GenerationContext {
public:
  GenerationContext()
  : m_offset(0), m_id(0)
  {}

  int offset() {
    return m_offset;
  }

  void increase(int n) {
    m_offset += n;
  }

  void reset_offset() {
    m_offset = 0;
  }

  void reset_offset(int n) {
    m_offset = n;
  }

  int id() {
    return ++m_id;
  }

  string* label() {
    string s = string("lbl_")+boost::lexical_cast<string>(id());
    return new string(s);
  }

private:
  int m_offset;
  int m_id;
};

/* Superclass for all nodes in the AST */
class Node
{
public:
  Node(int line)
  : m_location(line)
  {}

  Node()
  : m_location(0)
  {}

  virtual void resolve(SymbolTable* symtab) {}
  virtual void check_types() {}
  virtual void prepare(GenerationContext* ct) {}
  virtual void generate(GenerationContext* ct, ostream& os) {}

  virtual string dump() { return string("Node"); }

  int m_location;
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

  virtual bool is_indexed() {
    return false;
  }

  virtual Type* element_type();

  /* a op b : b->is_compatible(a) */
  /* a = b  : b->is_compatible(a) */
  virtual bool is_compatible(Type* t) = 0;
  virtual bool is_compatible_with(IntType* t) { return false; }
  virtual bool is_compatible_with(FloatType* t) { return false; }
  virtual bool is_compatible_with(StringType* t) { return false; }
  virtual bool is_compatible_with(VoidType* t) { return false; }
  virtual bool is_compatible_with(ArrayType* t) { return false; }
  virtual bool is_compatible_with(DimensionlessArrayType* t) { return false; }
  virtual bool is_compatible_with(ReferenceType* t) { return false; }
  virtual bool is_compatible_with(ErrorType* t) { return false; }

  virtual int size() = 0;
};


/* There is one object instance() that represents the int type */
class IntType : public Type
{
public:
  static IntType* instance() {
    return &s_instance;
  }

  virtual string dump() {
    return string("IntType");
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

  virtual bool is_compatible_with(IntType* t) {
    return true;
  }

  virtual int size() {
    return 4;
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

  virtual string dump() {
    return string("FloatType");
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

  virtual bool is_compatible_with(FloatType* t) {
    return true;
  }

  virtual int size() {
    return 8;
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

  virtual string dump() {
    return string("StringType");
  }


  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

  virtual bool is_compatible_with(StringType* t) {
    return true;
  }

  virtual int size() {
    return 8; /* Handle consisting of length + pointer to content */
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

  virtual string dump() {
    return string("VoidType");
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

  virtual int size() {
    return 0;
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

  ArrayType(int line, Type* base, int dimension);

  virtual string dump() {
    string s = boost::lexical_cast<string>(m_dim);
    return string("[")+s+string("]") + m_base->dump();
  }

  virtual bool is_indexed() {
    return true;
  }

  virtual Type* element_type() {
    return m_base;
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

  virtual bool is_compatible_with(ArrayType* t) {
    return m_dim==t->m_dim && t->m_base->is_compatible(m_base);
  }

  virtual int size() {
    return m_dim*m_base->size();
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

  virtual string dump() {
    return string("[]")+m_base->dump();
  }

  virtual bool is_indexed() {
    return true;
  }

  virtual Type* element_type() {
    return m_base;
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

  virtual bool is_compatible_with(DimensionlessArrayType* t) {
    return t->m_base->is_compatible(m_base);
  }

  virtual bool is_compatible_with(ArrayType* t) {
    return t->m_base->is_compatible(m_base);
  }

  virtual int size() {
    return 0; /* Placeholder since there are only references to dimensionless arrays */
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

  virtual string dump() {
    return string("*")+m_base->dump();
  }

  virtual Type* value_type() {
    return m_base;
  }

  virtual bool is_compatible(Type* t) {
    /* TODO : references to constants must not exist! */
    return t->is_compatible_with(this);
  }

  virtual bool is_compatible_with(IntType* t) {
    return t->is_compatible(m_base);
  }

  virtual bool is_compatible_with(FloatType* t) {
    return t->is_compatible(m_base);
  }

  virtual bool is_compatible_with(StringType* t) {
    return t->is_compatible(m_base);
  }

  virtual bool is_compatible_with(ArrayType* t) {
    return t->is_compatible(m_base);
  }

  virtual bool is_compatible_with(DimensionlessArrayType* t) {
    return t->is_compatible(m_base);
  }

  virtual int size() {
    return 4;
  }
};

/* This type marks a failed type check and avoids subsequent error messages */
class ErrorType : public Type
{
public:
  static ErrorType* instance() { return &s_instance; }

  virtual string dump() {
    return string("ErrorType");
  }

  virtual bool is_compatible(Type* t) {
    return t->is_compatible_with(this);
  }

  virtual bool is_compatible_with(Type* t) {
    return true;
  }

  virtual int size() {
    return 0;
  }

private:
  static ErrorType s_instance;
  ErrorType() {}
};

/* Superclass representing all kinds of variable declaration */
class VarDeclaration : public Node
{
public:
  string* m_name;
  Type* m_type;

  VarDeclaration(int line, string* name, Type* type)
  : Node(line), m_name(name), m_type(type)
  {}

};

/* Represents the declaration of a global variable */
class GlobalVarDeclaration : public VarDeclaration
{
public:
  int m_offset;

  GlobalVarDeclaration(int line, string* name, Type* type)
  : VarDeclaration(line,name,type)
  {}

  virtual void prepare(GenerationContext* ct);
  virtual void generate(GenerationContext* ct, ostream& os);
};

/* Represents a parameter in a function definition */
class ParamDeclaration : public VarDeclaration
{
public:
  ParamDeclaration(int line, string*name, Type* type)
  : VarDeclaration(line,name,type)
  {}

  int m_offset;
};

/* Represents a declaration of a local variable in a code block */
class LocalVarDeclaration : public VarDeclaration
{
public:
  LocalVarDeclaration(int line, string* name, Type* type)
  : VarDeclaration(line,name,type)
  {}

  int m_offset;
};

/* Superclass of all Expressions */
class Expr : public Node
{
public:
  Expr(int line)
  : Node(line)
  {}

  Type* m_type;
};

/* Superclass of variables, that can occur on the left side of an assignment, i.e.
   they are assignable */
class LValue : public Node
{
public:
  LValue(int line)
  : Node(line)
  {}

  Type* m_type;
};

/* Represents a variable name, e.g. a = ... or a[...]= ... */
class VarAccess : public LValue
{
public:
  string* m_name;
  VarDeclaration* m_decl;

  VarAccess(int line, string* name)
  : LValue(line), m_name(name), m_decl(NULL)
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
  : LValue(base->m_location), m_base(base), m_index(expr)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_base->resolve(symtab);
    m_index->resolve(symtab);
  }

  virtual void check_types();

  virtual void prepare(GenerationContext* ct) {
    m_base->prepare(ct);
    m_index->prepare(ct);
  }
};

/* Superclass of all Expressions with two children */
class BinaryExpr : public Expr
{
public:
  Expr* m_left;
  Expr* m_right;

  BinaryExpr(Expr* l, Expr* r)
  : Expr(l->m_location), m_left(l), m_right(r)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_left->resolve(symtab);
    m_right->resolve(symtab);
  }

  /* Valid for most subtypes ... */
  virtual void check_types() {
    m_left->check_types();
    m_right->check_types();
    m_type = ErrorType::instance();
  }

  virtual void prepare(GenerationContext* ct) {
    m_left->prepare(ct);
    m_right->prepare(ct);
  }
};

/* Superclass of all expressions with one child */
class UnaryExpr : public Expr
{
public:
  Expr* m_child;

  UnaryExpr(Expr* child)
  : Expr(child->m_location), m_child(child)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_child->resolve(symtab);
  }

  virtual void check_types() {
    m_child->check_types();
    m_type = ErrorType::instance();
  }

  virtual void prepare(GenerationContext* ct) {
    m_child->prepare(ct);
  }
};

class LogicalExpr : public BinaryExpr
{
public:
  LogicalExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}

  virtual void check_type();
};

class AndExpr : public LogicalExpr
{
public:
  AndExpr(Expr* l, Expr* r)
  : LogicalExpr(l,r)
  {}
};

class OrExpr : public LogicalExpr
{
public:
  OrExpr(Expr* l, Expr* r)
  : LogicalExpr(l,r)
  {}
};

class CompExpr : public BinaryExpr
{
public:
  CompExpr(Expr* l, Expr* r)
  : BinaryExpr(l,r)
  {}

  virtual void check_types();
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

  virtual void check_types();
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

  virtual void check_types();
};

class NegExpr : public UnaryExpr
{
public:
  NegExpr(Expr* child)
  : UnaryExpr(child)
  {}

  virtual void check_types();
};

class IntConversion : public UnaryExpr
{
public:
  IntConversion(Expr* expr)
  : UnaryExpr(expr)
  {}

  virtual void check_types();
};

class FloatConversion : public UnaryExpr
{
public:
  FloatConversion(Expr* expr)
  : UnaryExpr(expr)
  {}

  virtual void check_types();
};

/* Represents a string literal, such as "Hello Wordl!" */
class StringLiteral : public Expr
{
public:
  string* m_value;
  string* m_label;

  StringLiteral(int line, char *val)
  : Expr(line)
  {
    m_value = new string(val);
  }

  virtual void check_types() {
    m_type = StringType::instance();
  }

  virtual void prepare(GenerationContext* ct) {
    m_label = ct->label();
  }
};

/* Represents an integer literal, such as 4711 */
class IntLiteral : public Expr
{
public:
  int m_value;

  IntLiteral(int line, int val)
  : Expr(line), m_value(val)
  {}

  virtual void check_types() {
    m_type = IntType::instance();
  }

  virtual string dump() {
    string s = boost::lexical_cast<string>(m_value);

    return string("IntLiteral(")+s+")";
  }
};

/* Represents a float literal, such as 3.1415926 */
class FloatLiteral : public Expr
{
public:
  double m_value;

  FloatLiteral(int line, double val)
  : Expr(line), m_value(val)
  {}

  virtual void check_types() {
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

  FunctionCall(int line, string*name, list<Expr*>* args)
  : Expr(line), m_name(name), m_args(args), m_definition(NULL)
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
  : Expr(lval->m_location), m_lvalue(lval)
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
public:
  Stmt(int line)
  : Node(line)
  {}
};

/* Represents a sequence of variable declarations and statements in curly brackets */
class Block : public Node
{
public:
  list<Stmt*> m_stmts;
  list<LocalVarDeclaration*> m_declarations;
  int m_var_size;

  virtual void resolve(SymbolTable* symtab);
  virtual void check_types();
  virtual void prepare(GenerationContext* ct);
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
  int m_arg_size;

  FuncDefinition(int line, string* name, list<ParamDeclaration*>* params, Type* type, Block* block)
  : Node(line), m_name(name), m_params(params), m_type(type), m_stmts(block)
  {
    m_return_value = new ParamDeclaration(line,m_name,m_type);
  }

  virtual void resolve(SymbolTable* symtab);

  virtual void check_types() {
    m_stmts->check_types();
  }

  virtual void prepare(GenerationContext* ct);
  virtual void generate(GenerationContext* ct, ostream& os);
};

/* Represents an assignment with an lvalue and an expression */
class AssignmentStmt : public Stmt
{
public:
  LValue* m_lvalue;
  Expr* m_expr;

  AssignmentStmt(LValue* l, Expr* expr)
  : Stmt(l->m_location), m_lvalue(l), m_expr(expr)
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
  : Stmt(expr->m_location), m_expr(expr), m_true(t), m_false(f)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_expr->resolve(symtab);
    m_true->resolve(symtab);
    m_false->resolve(symtab);
  }

  virtual void check_types();

  virtual void prepare(GenerationContext* ct) {
    m_true->prepare(ct);
    m_false->prepare(ct);
  }
};

class WhileStmt : public Stmt
{
public:
  Expr* m_expr;
  Block* m_stmts;

  WhileStmt(Expr* expr, Block* stmts)
  : Stmt(expr->m_location), m_expr(expr), m_stmts(stmts)
  {}

  virtual void resolve(SymbolTable* symtab) {
    m_expr->resolve(symtab);
    m_stmts->resolve(symtab);
  }

  virtual void check_types();

  virtual void prepare(GenerationContext* ct) {
    m_stmts->prepare(ct);
  }
};

/* Represents a function call as a statement, i.e. it wraps a function call expression */
class FunctionCallStmt : public Stmt
{
public:
  FunctionCall* m_function;

  FunctionCallStmt(FunctionCall* function)
  : Stmt(function->m_location), m_function(function)
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
  void prepare();
  void generate(ostream& os);
};

extern Program* the_program;

#endif
