#include "Testcase.hpp"

#include "symtab.hpp"


class Node {
public:
  string m_name;
  Node(const char* name) :m_name(name) {}
  string* nm() { return &m_name; }
};

class SymTabTest : public Testcase
{
public:
  virtual void test() {
    SymbolTable s;
    Node* n;
    bool success;
    
    Node* n1 = new Node("Node1");
    success = s.insert(n1->nm(),n1);
    assert_true(success,"Inserting Node1 in empty table failed");
  
    Node* n2 = new Node("Node2");
    success = s.insert(n2->nm(),n2);
    assert_true(success,"Inserting Node2 in table with one element");

    success = s.insert(n1->nm(),n1);
    assert_false(success,"Checking for duplicate failed");

    s.enterScope();
        
    Node* n3 = new Node("Node3");
    success = s.insert(n3->nm(),n3);
    assert_true(success,"Inserting Node3 in new scope");
    
    Node* n4 = new Node("Node1");
    success = s.insert(n4->nm(),n4);
    assert_true(success,"Inserting new Node1 with same name in new scope");
    
    n = s.lookup(n3->nm());
    assert_true(n==n3,"Looking up Node3 in inner scope");
    
    n = s.lookup(n2->nm());
    assert_true(n==n2,"Looking up Node2 in outer scope");
    
    n = s.lookup(n1->nm());
    assert_true(n==n4,"Looking up Node1 in inner scope");
    
    n = new Node("NotInserted");
    n = s.lookup(n->nm());
    assert_true(n==NULL,"Looking up non-existent name");
    
    s.leaveScope();
    
    n = s.lookup(n4->nm());
    assert_true(n==n1,"Looking up Node1 after dropping inner scope");
    
    s.enterScope();
    s.insert(n4->nm(),n4);
    
    s.enterScope();
    s.insert(n4->nm(),n4);

    s.enterScope();
    s.insert(n4->nm(),n4);

    s.enterScope();
    s.insert(n1->nm(),n1);
    
    string tn = "Node1";
    n = s.lookup(&tn);
    assert_true(n==n1,"Nesting four scopes");
    
    string tn2 = "ThisNameDoesNotExists";
    n = s.lookup(&tn2);
    assert_true(n==NULL,"Nonexistent name in nested scopes");
    
    s.leaveScope();
    
    n = s.lookup(&tn);
    assert_true(n==n4,"Nesting many scopes, leaving one");
    
    s.leaveScope();
    s.leaveScope();
    s.leaveScope();
    
    n = s.lookup(&tn);
    assert_true(n==n1,"Leaving four scopes");
  }
};

main()
{
  SymTabTest().run();
}
