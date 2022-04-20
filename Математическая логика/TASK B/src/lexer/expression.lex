%{
#include <string>
#include "../exp.h"
#include "expression.tab.h"
%}

%option outfile="lex.c" header-file="lex.h"


white [ \t]+
digit [0-9]
letter [A-Z]
chars '
symbol {letter}({letter}|{digit}|{chars})*

%%
{white}
{symbol} {
    yylval.name=new std::string(yytext);
    return NAME;
}
"&" return AND;
"|" return OR;
"->" return IMP;
"!" return NEG;
"(" return L;
")" return R;
"\n" return END;
%%
