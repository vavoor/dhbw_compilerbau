#include <stdio.h>
#include <string>
using namespace std;

#include "parser_inc.hpp"
#include "lex.yy.h"
#include "err.hpp"

int main(int argc, const char* argv[])
{
  int yyparse(void);

  if (argc==2) {
    yyin = fopen(argv[1],"r");
    if (yyin==NULL) {
      fatal(0,"Cannot open input file %s",argv[1]);
    }
    yyparse();
    the_program->resolve();
    fprintf(stdout,"Done\n");
  }
  return 0;
}
