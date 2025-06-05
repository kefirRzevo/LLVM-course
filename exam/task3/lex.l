%{
#include "parser.tab.h"
%}

%option noyywrap

DOT    "."
NUM    (0|[1-9][0-9]*)

%%
{NUM}  { yylval = atoi(yytext);
  #ifdef DEBUG
  printf("Num: %d\n", yylval);
  #endif
  return (unsigned) NUM; }

{DOT} { return DOT; }

[ \t\n]    ; // Whitespace

. { printf("unknown token <%s>\n", yytext); exit(1); }

%%
