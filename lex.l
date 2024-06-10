%option noyywrap never-interactive

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parse.tab.h"

void yyerror(char *s) { fprintf(stderr, "%s\n",s); }
%}

%%

[ \t\n\r] ;
"circuit" return CIRCUIT;
"let"     return LET;
"output"  return OUTPUT;
[_a-zA-Z0-9]+    { yylval.id = strdup(yytext); return ID; }
"("       return LPAREN;
")"       return RPAREN;
","       return COMMA;
"$"       return DOLLAR;
"{"       return LCURLY;
"}"       return RCURLY;
"="       return EQ;
<<EOF>>   return EOI;
.         { yyerror("Illegal character"); exit(1); }
