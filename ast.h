#ifndef AST_H
#define AST_H

#include "gate.h"

typedef enum {
    CIRCUIT_DEF = 0,
    VAR,
    GATE,
    LET_VAR,
    OUTPUT_DEF
} ntype;

struct AST {
    ntype type;
    char *id;
    char *var;
    int inputNum;
    char **input;
    operator op;
    int argsNum;
    struct AST** args;
};

typedef struct AST *AST;

struct ASTList {
    int n;
    AST *list;
};

struct symbolList {
    int n;
    char **list;
};
typedef struct symbolList *symbolList;

typedef struct ASTList *ASTList;

AST Var(char *);
AST Gate(operator, int, AST*);
AST InputList(int, AST*);
AST Let(char *, AST);
AST CircuitDef(char *, symbolList, ASTList);
AST OutputDef(AST);
AST makeGateNode(char *, ASTList);
symbolList newSymbolList();
symbolList addNewSymbol(symbolList, char *);
ASTList newList(int);
ASTList addLast(ASTList, AST);
void printAST(int, AST);

#endif
