
%{

#include "parser.tab.hh"
#include "frontend/Lexer.hh"
#include "frontend/Driver.hh"

#include <cstdlib>

%}

%option noyywrap nounput noinput nodefault 
%option c++
%option yyclass="Lexer"
%option nounistd

%x comment_multiline
number          (0|[1-9][0-9]*)
identifier      [a-zA-Z_][a-zA-Z_0-9]*
comment_single  ([/][/].*)

%%

"/*"                            { BEGIN(comment_multiline); updateLocation(); }
<comment_multiline>[^*\n]*      { updateLocation(); }
<comment_multiline>"*"+[^*/\n]* { updateLocation(); }
<comment_multiline>\n|\r\n      { curPos_.lines(yyleng); updateLocation(); }
<comment_multiline>"*"+"/"      { BEGIN(INITIAL); updateLocation(); }

<comment_multiline><<EOF>>      {
  auto loc = updateLocation();
  driver_.reportError<UnterminatedComment>(loc);
  return Parser::make_EOF(loc);
}

"break"         { return Parser::make_BREAK(updateLocation()); }
"continue"      { return Parser::make_CONTINUE(updateLocation()); }
"else"          { return Parser::make_ELSE(updateLocation()); }
"if"            { return Parser::make_IF(updateLocation()); }
"while"         { return Parser::make_WHILE(updateLocation()); }
"print"         { return Parser::make_PRINT(updateLocation()); }
"int"           { return Parser::make_INT(updateLocation()); }
"void"          { return Parser::make_VOID(updateLocation()); }
"undef"         { return Parser::make_UNDEF(updateLocation()); }
"func"          { return Parser::make_FUNC(updateLocation()); }
"return"        { return Parser::make_RETURN(updateLocation()); }
"&&"            { return Parser::make_AND_OP(updateLocation()); }
"&"             { return Parser::make_BIT_AND(updateLocation()); }
"||"            { return Parser::make_OR_OP(updateLocation()); }
"|"             { return Parser::make_BIT_OR(updateLocation()); }
"^"             { return Parser::make_BIT_XOR(updateLocation()); }
"<="            { return Parser::make_LE_OP(updateLocation()); }
">="            { return Parser::make_GE_OP(updateLocation()); }
">>"            { return Parser::make_RIGHT_OP(updateLocation()); }
">"             { return Parser::make_G_OP(updateLocation()); }
"<<"            { return Parser::make_LEFT_OP(updateLocation()); }
"<"             { return Parser::make_L_OP(updateLocation()); }
"=="            { return Parser::make_EQ_OP(updateLocation()); }
"!="            { return Parser::make_NE_OP(updateLocation()); }
";"             { return Parser::make_SEMICOL(updateLocation()); }
","             { return Parser::make_COMMA(updateLocation()); }
":"             { return Parser::make_COLON(updateLocation()); }
"="             { return Parser::make_ASSIGN(updateLocation()); }
"["             { return Parser::make_LSQUARE(updateLocation()); }
"]"             { return Parser::make_RSQUARE(updateLocation()); }
"("             { return Parser::make_LPAREN(updateLocation()); }
")"             { return Parser::make_RPAREN(updateLocation()); }
"{"             { return Parser::make_LCURLY(updateLocation()); }
"}"             { return Parser::make_RCURLY(updateLocation()); }
"!"             { return Parser::make_NOT_OP(updateLocation()); }
"?"             { return Parser::make_QMARK(updateLocation()); }
"+"             { return Parser::make_ADD_OP(updateLocation()); }
"-"             { return Parser::make_SUB_OP(updateLocation()); }
"*"             { return Parser::make_MUL_OP(updateLocation()); }
"/"             { return Parser::make_DIV_OP(updateLocation()); }
"%"             { return Parser::make_MOD_OP(updateLocation()); }

{identifier}      { return Parser::make_IDENTIFIER(yytext, updateLocation()); }
{number}          { return Parser::make_CONSTANT(std::atoi(yytext), updateLocation()); }
{comment_single}  { updateLocation(); }

[\n|\r\n]+  { curPos_.lines(yyleng); updateLocation(); }
[ \t]+      { curPos_.columns(yyleng); updateLocation(); }

. {
  auto loc = updateLocation();
  driver_.reportError<UnknownToken>(loc, yytext);
}

<<EOF>>         { return Parser::make_EOF(updateLocation()); }

%%
