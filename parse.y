%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "term.h"

extern int yylex();
extern void yyerror(char *);
%}

%union { char *name; term term;}

%token COMMA LPAREN RPAREN DOLLAR EOI
%token <name> ID

%type <term> term
%start toplevel

%%

toplevel : term EOI       { print($1); printf("\n");
                            YYACCEPT; }

term : DOLLAR ID                        { $$ = var($2); }
     | ID                               { $$ = con($1);}
     | ID LPAREN term COMMA term RPAREN { $$ = fun($1, $3, $5); }
     ;

%%

int main(int argc, char **argv)
{
  yyparse();
  return 0;
}
