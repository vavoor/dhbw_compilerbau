#include <stdio.h>
#include <stdarg.h>

void fatal(int lineno, const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  if (lineno>0) {
    fprintf(stderr,"Line %d: ",lineno);
  }
  vfprintf(stderr,msg,args);
  fprintf(stderr,"\n");
  va_end(args);
}
