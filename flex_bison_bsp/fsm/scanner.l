%{
/*
  Translate with
  flex -B8 --header-file=lex.yy.h scanner.l
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"
%}

%option noyywrap

%%

"initial"       { return T_INITIAL; }
"state"         { return T_STATE; }
"on"            { return T_ON; }
"goto"          { return T_GOTO; }
"control"       { return T_CONTROL; }
":"             { return ':'; }

[[:alnum:]_]+   { yylval.s = strdup(yytext); return T_IDENTIFIER; }

"#".*           /* ignore comments */
[[:space:]]     /* ignore spaces */

.               { fprintf(stderr,"Illegal character\n"); exit(1); }

%%