%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
int yyparse();
void yyerror(const char *s) {
  fprintf(stderr, "ip <%s> is incorrect\n", s);
  exit(1);
}
int validate_ip_part(unsigned num);

%}

%token NUM
%token DOT

%glr-parser
%expect-rr 0

%%

prog: data |
  prog data ;

data: NUM DOT NUM DOT NUM DOT NUM {
  if (validate_ip_part($1) || validate_ip_part($3) || validate_ip_part($5) || validate_ip_part($7)) {
    yyerror("bad values");
  }
  printf("ip <%d.%d.%d.%d> is correct\n", $1, $3, $5, $7);
  return yyok;
}

%%

int validate_ip_part(unsigned num) {
  if (num < 0 || num > 255)
    return 1;
  return 0;
}

int main() {
  yyparse();
}
