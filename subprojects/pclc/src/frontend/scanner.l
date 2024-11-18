%{

#include "parser_generated.hpp"
#include "frontend/scanner.hpp"

#include <iostream>
#include <string>
#include <sstream>

%}

%option noyywrap nounput noinput nodefault
%option c++
%option prefix="paracl_"
%option yyclass="scanner"
%option nounistd

number      (0|[1-9][0-9]*)
identifier  [a-zA-Z_][a-zA-Z_0-9]*

cpp_style_comment ([/][/].*)

%%

"int"           { return parser::make_INT(update_loc()); }
"void"          { return parser::make_VOID(update_loc()); }

"while"         { return parser::make_WHILE(update_loc()); }
"if"            { return parser::make_IF(update_loc()); }
"else"          { return parser::make_ELSE(update_loc()); }
"print"         { return parser::make_PRINT(update_loc()); }
"func"          { return parser::make_FUNC(update_loc()); }
"return"        { return parser::make_RET(update_loc());}

"("             { return parser::make_LPAREN(update_loc()); }
")"             { return parser::make_RPAREN(update_loc()); }
"["             { return parser::make_LBRACKET(update_loc()); }
"]"             { return parser::make_RBRACKET(update_loc()); }
"{"             { return parser::make_LBRACE(update_loc()); }
"}"             { return parser::make_RBRACE(update_loc()); }
";"             { return parser::make_SEMICOL(update_loc()); }
":"             { return parser::make_COL(update_loc()); }
","             { return parser::make_COMMA(update_loc()); }

"=="            { return parser::make_COMP_EQ(update_loc()); }
"!="            { return parser::make_COMP_NE(update_loc()); }
">"             { return parser::make_COMP_GT(update_loc()); }
"<"             { return parser::make_COMP_LS(update_loc()); }
">="            { return parser::make_COMP_GE(update_loc()); }
"<="            { return parser::make_COMP_LE(update_loc()); }

"&&"            { return parser::make_LOGICAL_AND(update_loc()); }
"||"            { return parser::make_LOGICAL_OR(update_loc()); }

"<<"            { return parser::make_LEFT_SHIFT(update_loc()); }
">>"            { return parser::make_RIGHT_SHIFT(update_loc()); }

"!"             { return parser::make_BANG(update_loc()); }
"?"             { return parser::make_QMARK(update_loc()); }

"+"             { return parser::make_PLUS(update_loc()); }
"-"             { return parser::make_MINUS(update_loc()); }
"*"             { return parser::make_MULTIPLY(update_loc()); }
"/"             { return parser::make_DIVIDE(update_loc()); }
"%"             { return parser::make_MODULUS(update_loc()); }

"="             { return parser::make_ASSIGN(update_loc()); }

{identifier}    { auto newloc = update_loc(); return parser::make_IDENTIFIER(yytext, newloc); }
{number}        { auto newloc = update_loc(); return parser::make_INTEGER_CONSTANT(std::atoi(yytext), newloc); }

[\n|\r\n]+      { m_pos.lines(yyleng); }
[ \t]+          { m_pos.columns(yyleng); }

{cpp_style_comment} { update_loc(); }

. {
  auto loc = update_loc();
  std::stringstream ss;
  ss << "Unknown character '" << yytext << "'";
  auto msg = ss.str();
  m_driver.report_error(msg, loc); throw parser::syntax_error{loc, msg};
}

<<EOF>> { return paracl::frontend::parser::make_EOF(update_loc()); }

%%
