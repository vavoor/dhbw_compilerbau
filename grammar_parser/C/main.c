#include <stdio.h>
#include "parser.h"

int main(int argc, const char* argv[])
{
  if (argc==2) {
    parse_grammar(argv[1]);
  }
  else {
    fprintf(stderr,"Usage: %s FILENAME\n",argv[0]);
  }

  return 0;
}
