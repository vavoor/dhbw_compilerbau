#include "Testcase.hpp"

#include "symtab.hpp"


class VarDeclaration
{
public:
  string m_name;
  VarDeclaration(const char* name) :m_name(name) {}
  string* nm() { return &m_name; }
};


class FuncDefinition
{
public:
  string m_name;
  FuncDefinition(const char* name) :m_name(name) {}
  string* nm() { return &m_name; }
};


class SymTabTest : public Testcase
{
public:
  virtual void test() {
    SymbolTable s;
    VarDeclaration* n;
    bool success;

    VarDeclaration* n1 = new VarDeclaration("VarDeclaration1");
    success = s.insertVariable(n1->nm(),n1);
    assert_true(success,"insertVariableing VarDeclaration1 in empty table failed");

    VarDeclaration* n2 = new VarDeclaration("VarDeclaration2");
    success = s.insertVariable(n2->nm(),n2);
    assert_true(success,"insertVariableing VarDeclaration2 in table with one element");

    success = s.insertVariable(n1->nm(),n1);
    assert_false(success,"Checking for duplicate failed");

    s.enterScope();

    VarDeclaration* n3 = new VarDeclaration("VarDeclaration3");
    success = s.insertVariable(n3->nm(),n3);
    assert_true(success,"insertVariableing VarDeclaration3 in new scope");

    VarDeclaration* n4 = new VarDeclaration("VarDeclaration1");
    success = s.insertVariable(n4->nm(),n4);
    assert_true(success,"insertVariableing new VarDeclaration1 with same name in new scope");

    n = s.lookupVariable(n3->nm());
    assert_true(n==n3,"Looking up VarDeclaration3 in inner scope");

    n = s.lookupVariable(n2->nm());
    assert_true(n==n2,"Looking up VarDeclaration2 in outer scope");

    n = s.lookupVariable(n1->nm());
    assert_true(n==n4,"Looking up VarDeclaration1 in inner scope");

    n = new VarDeclaration("NotinsertVariableed");
    n = s.lookupVariable(n->nm());
    assert_true(n==NULL,"Looking up non-existent name");

    s.leaveScope();

    n = s.lookupVariable(n4->nm());
    assert_true(n==n1,"Looking up VarDeclaration1 after dropping inner scope");

    s.enterScope();
    s.insertVariable(n4->nm(),n4);

    s.enterScope();
    s.insertVariable(n4->nm(),n4);

    s.enterScope();
    s.insertVariable(n4->nm(),n4);

    s.enterScope();
    s.insertVariable(n1->nm(),n1);

    string tn = "VarDeclaration1";
    n = s.lookupVariable(&tn);
    assert_true(n==n1,"Nesting four scopes");

    string tn2 = "ThisNameDoesNotExists";
    n = s.lookupVariable(&tn2);
    assert_true(n==NULL,"Nonexistent name in nested scopes");

    s.leaveScope();

    n = s.lookupVariable(&tn);
    assert_true(n==n4,"Nesting many scopes, leaving one");

    s.leaveScope();
    s.leaveScope();
    s.leaveScope();

    n = s.lookupVariable(&tn);
    assert_true(n==n1,"Leaving four scopes");

    FuncDefinition* f1 = new FuncDefinition("Function1");
    success = s.insertFunction(f1->nm(),f1);
    assert_true(success,"Entering first function");

    FuncDefinition* f2 = new FuncDefinition("Function2");
    success = s.insertFunction(f2->nm(),f2);
    assert_true(success,"Entering second function");

    FuncDefinition* f;

    string fn1 = "Function1";
    f = s.lookupFunction(&fn1);
    assert_true(f==f1,"Looking up first function");

    string fn2 = "Function2";
    f = s.lookupFunction(&fn2);
    assert_true(f==f2,"Looking up second function");

    string fn3 = "NonExistentFunction";
    f = s.lookupFunction(&fn3);
    assert_true(f==NULL,"Looking up non-existent name");
  }
};

int main()
{
  SymTabTest().run();
  return 0;
}
