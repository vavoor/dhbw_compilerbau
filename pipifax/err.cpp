#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

static int err_count = 0;

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
  err_count++; 
  exit(1);
}

void err(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vfprintf(stderr,msg,args);
  fprintf(stderr,"\n");
  va_end(args);
  err_count++;
}

int get_error_count()
{
  return err_count;
}
