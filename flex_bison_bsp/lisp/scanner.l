%{
#include <string.h>
#include <stdlib.h>

#include "list.h"
#include "parser.tab.h"
%}

D   [0-9]
S   [A-Za-z!$%&/=?+*#\-_.:<>]

%option noyywrap yylineno

%%

{S}+    { yylval.string = strdup(yytext); return SYMBOL; }
{D}+    { yylval.number = atoi(yytext); return NUMBER; }

"("     { return yytext[0]; }
")"     { return yytext[0]; }


";"[^\n]*  /* ignore */
[[:space:]] /* ignore */

%%
