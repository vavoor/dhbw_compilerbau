#ifndef ID124420ee_f994_42c9_9661_5ece15901dcc
#define ID124420ee_f994_42c9_9661_5ece15901dcc

#include <iostream>
using namespace std;

class Testcase
{
public:
  virtual void setup() {}
  virtual void teardown() {}
  virtual void test() = 0;

  Testcase()
  : m_passed(true)
  {}

  void run() {
    setup();
    test();
    teardown();
    if (!m_passed) {
      cerr << "F.A.I.L." << endl;
    }
  }

protected:
  void assert_true(bool res, const string& msg)
  {
    cerr << msg << ": ";
    if (res) {
      cerr << "PASS" << endl;
    }
    else {
      m_passed = false;
      cerr << "FAIL" << endl;
    }
  }

  void assert_false(bool res, const string& msg)
  {
    assert_true(!res,msg);
  }

private:
  bool m_passed;
};

#endif
