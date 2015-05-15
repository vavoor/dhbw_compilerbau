#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

#include "parser_inc.hpp"
#include "lex.yy.h"
#include "err.hpp"

int main(int argc, const char* argv[])
{
  int errors;
  int yyparse(void);
  extern int yydebug;
  //yydebug = 1;

  if (argc==2) {
    yyin = fopen(argv[1],"r");
    if (yyin==NULL) {
      fatal(0,"Cannot open input file %s",argv[1]);
    }
    errors = yyparse();
    if (errors==0) {
      the_program->resolve();
      errors = get_error_count();
    }
    if (errors==0) {
      the_program->check_types();
      errors = get_error_count();
    }
    if (errors==0) {
      the_program->prepare();
      errors = get_error_count();
    }
    if (errors==0) {
      the_program->generate(cout);
      errors = get_error_count();
    }

    if (errors==0) {
      fprintf(stdout,"Done\n");
    }
    else {
      fprintf(stdout,"FAILED with %d errors\n",errors);
    }
  }
  return 0;
}
