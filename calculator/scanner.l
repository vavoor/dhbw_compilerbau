/*
  flex --header-file=lex.yy.h -8B scanner.l
*/

%{
#include "parser.tab.h"
%}

%option noyywrap

%%

[[:space:]]         /* ignore */

%%
