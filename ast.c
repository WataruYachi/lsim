#include <gc/gc.h>
#include <gc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "gate.h"

AST Var(char *v) {
    AST n = GC_MALLOC(sizeof(struct AST));
    n->type = VAR;
    n->var = v;
    n->op = -1;
    n->argsNum = 0;
    n->args = NULL;
    return n;
}

AST Gate(operator op, int argsNum, AST *args) {
    AST n = GC_MALLOC(sizeof(struct AST));
    n->type = GATE;
    n->var = NULL;
    n->op = op;
    n->argsNum = argsNum; 
    n->args = args;
    return n;
}

AST Let(char *v, AST arg) {
    AST n = GC_MALLOC(sizeof(struct AST));
    AST* l = GC_MALLOC(sizeof(AST));
    l[0] = arg;
    n->type = LET_VAR;
    n->var = v;
    n->op = -1;
    n->argsNum = 1;
    n->args = l;
    return n;
}

AST CircuitDef(char *id, symbolList input, ASTList args) {
    AST n = GC_MALLOC(sizeof(struct AST));
    n->type = CIRCUIT_DEF;
    n->id = id;
    n->var = NULL;
    n->inputNum = input->n;
    n->input = input->list;
    n->op = -1;
    n->argsNum = args->n;
    n->args = args->list;
    return n;
}

AST OutputDef(AST g) {
    AST n = GC_MALLOC(sizeof(struct AST));
    AST * l = GC_MALLOC(sizeof(AST)); 
    l[0] = g;
    n->type = OUTPUT_DEF;
    n->var = NULL;
    n->op = OUTPUT_GATE;
    n->argsNum = 1; 
    n->args = l;
    return n;
}

AST makeGateNode(char *id, ASTList args) {
    AST g;
    if (strcmp(id, "and") == 0) {
        g = Gate(AND_GATE, args->n, args->list);
    } else if (strcmp(id, "or") == 0) {
        g = Gate(OR_GATE, args->n, args->list);
    } else if (strcmp(id, "not") == 0) {
        g = Gate(NOT_GATE, args->n, args->list);
    } else if (strcmp(id, "xor") == 0) {
        g = Gate(XOR_GATE, args->n, args->list);
    } else if (strcmp(id, "nand") == 0) {
        g = Gate(NAND_GATE, args->n, args->list);
    } else if (strcmp(id, "nor") == 0) {
        g = Gate(NOR_GATE, args->n, args->list);
    } else {
        fprintf(stderr, "undefined gate:%s\n", id);
        exit(1);
    }
    return g;
}

symbolList newSymbolList() {
    symbolList l = GC_MALLOC(sizeof(struct symbolList));
    l->list = GC_MALLOC(0);
    l->n = 0;
    return l;
}

symbolList addNewSymbol(symbolList l, char *a) {
    char **nl = GC_REALLOC(l->list, sizeof(char *) * (l->n + 1));
    nl[l->n] = a;
    l->list = nl;
    l->n += 1;
    return l; 
}

ASTList newList(int n) {
    ASTList l = GC_MALLOC(sizeof(struct ASTList));
    AST *list = GC_MALLOC(sizeof(AST)*n);
    l->n = n;
    l->list = list;
    return l;
}

ASTList addLast(ASTList l, AST a) {
    AST *nl = GC_REALLOC(l->list, sizeof(AST) * (l->n + 1));
    nl[l->n] = a;
    l->list = nl;
    l->n += 1;
    return l;
}

void indent(int n) {
    for (int i =0; i < n; i++) {
        printf("  ");
    }
}

void printAST(int n, AST node) {
    indent(n);
    printf("type:%d\n",node->type);
    switch (node->type) {
        case CIRCUIT_DEF:
            indent(n);
            printf("id:%s\n", node->id);
            indent(n);
            printf("input:");
            for (int i = 0; i < node->inputNum; i++) {
                printf("%s,", node->input[i]);
            }
            printf("\n");
            for(int i = 0; i < node->argsNum; i++) {
                printAST(n+1, node->args[i]);
            }
            break;
        case VAR:
            indent(n);
            printf("var:%s\n", node->var);
            break;
        case GATE:
            indent(n);
            printf("op:%d\n", node->op);
            for(int i = 0; i < node->argsNum; i++) {
                printAST(n+1, node->args[i]);
            }
            break;
        case LET_VAR:
            indent(n);
            printf("var:%s\n", node->var);
            printAST(n+1, node->args[0]);
            break; 
        case OUTPUT_DEF:
            indent(n);
            printf("output\n");
            printAST(n+1, node->args[0]);
            break;
    } 
}



