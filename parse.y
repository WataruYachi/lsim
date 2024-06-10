%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "term.h"
#include "gate.h"
#include "circuit.h"
#include "table.h"
#include "bit.h"
#include "qm.h"
#include "builder.h"

extern int yylex();
extern void yyerror(char *);
AST result;
%}

%union {char *id; symbolList slist; AST ast; ASTList list;}

%token COMMA LPAREN RPAREN DOLLAR EQ EOI
%token LCURLY RCURLY CIRCUIT LET OUTPUT
%token <id> ID
%type <ast> circuit
%type <slist> inputVars
%type <list> block
%type <ast> line
%type <ast> term
%type <list> termList  
%type <ast> toplevel
%start toplevel

%%

toplevel : circuit EOI      { result = $1; YYACCEPT;}; //result = $1; }
circuit : CIRCUIT ID LPAREN inputVars RPAREN LCURLY block RCURLY { $$ = CircuitDef($2, $4, $7); };

inputVars : /* */ {$$ = newSymbolList();}
          | ID                  { $$ = addNewSymbol(newSymbolList(), $1);}
          | inputVars COMMA ID  { $$ = addNewSymbol($1, $3);}
          ;

block : line       {$$ = addLast(newList(0), $1);}
      | block line {$$ = addLast($1, $2);}
      ;

line : LET ID EQ term {$$ = Let($2, $4);}
     | OUTPUT LPAREN term RPAREN {$$ = OutputDef($3);}
     ;

term : DOLLAR ID                        { $$ = Var($2); }
     | ID LPAREN termList RPAREN { $$ = makeGateNode($1, $3); }
     ;

termList : term {$$ = addLast(newList(0), $1);}
         | termList COMMA term {$$ = addLast($1, $3);}
         ;
%%

void usage() {
    printf("usage\n");
    printf("./lc <option> <file>\n");
    printf("options:\n");
    printf(" -m : minimalize input circuit (only single output circuit)\n");
}

int main (int argc, char *argv[]) {
    extern FILE *yyin;

    if (argc <= 1) {
        usage();
        return 0;
    }

    int mf = 0;
    char *option;
    char *file;
    
    if (argc == 3) {
        option = argv[1];
        file = argv[2];
        if (strcmp(option, "-m") != 0) {
            printf("unknow option:%s\n", option);
            return 1;
        } else {
            mf = 1;
        }
    } else if (argc == 2) {
        file = argv[1];
    }



    yyin = fopen(file, "r");
    if (yyin == NULL) {
        fprintf(stderr, "cannot open file:%s\n", file);
        exit(1);
    }

    if (yyparse()) {
        fprintf(stderr, "parse error\n");
        exit(1);
    }
    //printf("parse succesed\n");
    //printAST(0,result);
    circuit c = circuitBuilder(result);
    printf("truth table of :%s\n", c->id);
    truthTable tt = makeTruthTable(c);
    printTruthTable(tt);
    
    if (mf) {
        minimalizeCircuit(c);
    }

    fclose(yyin);
    return 0;
}
