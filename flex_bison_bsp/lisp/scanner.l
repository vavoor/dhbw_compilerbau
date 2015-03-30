%{
#include "parser.tab.h"
%}

D   [0-9]
S   [A-Za-z!$%&/=?+*#\-_.:<>]

%option noyywrap yylineno

%%

{S}+    { return SYMBOL; }
{D}+    { return NUMBER; }

"("     { return yytext[0]; }
")"     { return yytext[0]; }


";"[^\n]*  /* ignore */
[[:space:]] /* ignore */

%%
