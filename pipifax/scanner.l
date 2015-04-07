%{
#include <string.h>
#include <stdlib.h>
#include "parser.tab.h"
%}

L   [A-Za-z_]
D   [0-9]
LD  ({L}|{D})
ID  {L}{LD}*

%option yylineno noyywrap

%%

"var"   { return T_VAR; }
"func"  { return T_FUNC; }
"if"    { return T_IF; }
"while" { return T_WHILE; }
"int"   { return T_INT; }
"float" { return T_FLOAT; }
"string" { return T_STRING; }

{D}+    { yylval.i = atoi(yytext); return T_INT_VALUE; }
{D}+"."{D}+ { yylval.d = atof(yytext); return T_FLOAT_VALUE; }
{ID}    { yylval.s = strdup(yytext); return T_IDENT; }


.       { return yytext[0]; }

[[:space:]] /* ignore */

