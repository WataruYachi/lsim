%option noyywrap never-interactive

%{
#include <stdio.h>
#include <stdlib.h>
#include "term.h"
#include "parse.tab.h"

void yyerror(char *s) { fprintf(stderr, "%s\n",s); }
%}

%%

[ \t\n\r] ;
[_a-zA-Z0-9]+    { yylval.name = strdup(yytext); return ID; }
"("       return LPAREN;
")"       return RPAREN;
","       return COMMA;
"$"       return DOLLAR;
<<EOF>>   return EOI;
.         { yyerror("Illegal character"); exit(1); }
