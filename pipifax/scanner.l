%{
#include <string.h>
#include <stdlib.h>

#include "parser_inc.hpp"

static char* unescape(const char* s);
static void p(void);
static int yy_lex_debug = 0;
%}

L   [A-Za-z_]
D   [0-9]
LD  {L}|{D}
ID  {L}{LD}*
E   [eE]("+"|"-")?{D}+
I   {D}+
F   {I}("."{I})?{E}?
S   "\""("\\".|[^\\"])*"\""

%option yylineno noyywrap

%%

"var"         { p(); return T_VAR; }
"func"        { p(); return T_FUNC; }
"if"          { p(); return T_IF; }
"else"        { p(); return T_ELSE; }
"while"       { p(); return T_WHILE; }
"int"         { p(); return T_INT; }
"float"       { p(); return T_FLOAT; }
"string"      { p(); return T_STRING; }
"<="          { p(); return T_LE; }
">="          { p(); return T_GE; }
"=="          { p(); return T_EQ; }
"!="          { p(); return T_NE; }
"&&"          { p(); return T_AND; }
"||"          { p(); return T_OR; }

{I}           { p(); yylval.i = atoi(yytext); return T_INT_VALUE; }
{F}           { p(); yylval.d = atof(yytext); return T_FLOAT_VALUE; }
{ID}          { p(); yylval.s = strdup(yytext); return T_IDENT; }
{S}           { p(); yylval.s = unescape(yytext); return T_STRING_VALUE; }

"#".*         /* ignore */
[[:space:]]   /* ignore */
.             { p(); return yytext[0]; }

%%

static char* unescape(const char* s)
{
  char* str = (char*) malloc(strlen(s));
  const char* f = &s[1];
  char* t = str;

  while (*f!=s[0]) {
    if (*f=='\\') {
      f++;
    }
    *t = *f;
    t++;
    f++;
  }
  *t = '\0';

  return str;
}

static void p(void)
{
  if (yy_lex_debug) {
    fprintf(stderr,"\'%s\' found\n",yytext);
  }
}

