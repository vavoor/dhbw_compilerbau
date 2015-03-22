#include <stdio.h>
#include <stdarg.h>

#include "msg.h"

static int debug_flag = 0;
static errors = 0;


void setDebugFlag(void)
{
  debug_flag = 1;
}


void errmsg(int lineno, const char* fmt, ...)
{
  va_list ap;
  va_start(ap,fmt);

  errors++;

  if (lineno>0) {
    fprintf(stderr,"Line %d: ",lineno);
  }

  vfprintf(stderr,fmt,ap);
  fprintf(stderr,"\n");

  va_end(ap);
}

void debmsg(const char* fmt, ...)
{
  if (debug_flag) {
    va_list ap;
    va_start(ap,fmt);

    fprintf(stderr,"DEBUG: ");
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");

    va_end(ap);
  }
}

int errorcount(void)
{
  return errors;
}


void reset_error_count(void)
{
  errors = 0;
}


int isDebug(void)
{
  return debug_flag;
}

