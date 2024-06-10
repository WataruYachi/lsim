#include <gc/gc.h>
#include <gc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builder.h"
#include "gate.h"
#include "circuit.h"
#include "ast.h"


assignment Assignment(char *v, gate g) {
    assignment a = GC_MALLOC(sizeof(struct assignment));
    a->var = v;
    a->g = g;
    return a;
}

env newEnv() {
    env e = GC_MALLOC(sizeof(struct env));
    e->n = 0;
    e->a = NULL;
    return e;
}

void addPair(env e, assignment a) {
    e->a = GC_REALLOC(e->a, sizeof(struct env) * (e->n + 1));
    e->a[e->n] = a;
    e->n += 1;
}

gate lookup(env e, char *v) {
    for (int i = 0; i < e->n; i++) {
        if (strcmp(e->a[i]->var, v) == 0) {
            return e->a[i]->g;
        }
    }
    return NULL;
}

circuit circuitBuilder(AST node) {
    gate o;
    assignment a;

    if (node->type != CIRCUIT_DEF) {
        fprintf(stderr, "invalid input");
        exit(1);
    }

    env e = newEnv();

    gate *input = inputArray(node->inputNum, node->input);

    for (int i = 0; i < node->inputNum; i++) {
        a = Assignment(node->input[i], input[i]);
        addPair(e, a);
    }

    gate *output = gateArray(0);
    int outputNum = 0;
    for (int i = 0; i < node->argsNum; i++) {
        switch (node->args[i]->type) {
            case LET_VAR:
                if (lookup(e, node->args[i]->var) != NULL) {
                    fprintf(stderr, "multipul declare:%s\n", node->args[i]->var);
                    exit(1);
                }
                a = Assignment(node->args[i]->var, gateBuilder(e, node->args[i]->args[0]));
                addPair(e, a);
                break;
            case OUTPUT_DEF:
                o = gateBuilder(e, node->args[i]->args[0]);
                output = addArray(outputNum, output, Output(node->args[i]->args[0]->var, o));
                outputNum++;
                break;
            default:
                fprintf(stderr, "error in circuitBuilder\n");
                exit(1);
                break;
        }
    }
    circuit c =  Circuit(node->id, node->inputNum, input, outputNum, output);
    return c;
}

gate gateBuilder(env e, AST node) {
    gate *args;
    gate g;
    switch (node->type) {
        case VAR:
            g = lookup(e, node->var);
            if (g != NULL) {
                return g;
            } else {
                fprintf(stderr, "undeclared variable: %s", node->var);
                exit(1);
            }
            break;
        case GATE:
            args = gateArray(node->argsNum);
            for (int i = 0; i < node->argsNum; i++) {
                args[i] = gateBuilder(e, node->args[i]);
            }
            switch (node->op) {
                case AND_GATE:
                    return And("and", node->argsNum, args);
                break;
                case OR_GATE:
                    return Or("or", node->argsNum, args);
                break;
                case NOT_GATE:
                    return Not("not", args[0]);
                break;
                case XOR_GATE:
                    return Xor("xor", node->argsNum, args);
                break;
                case NAND_GATE:
                    return Nand("nand", node->argsNum, args);
                break;
                case NOR_GATE:
                    return Nor("nor", node->argsNum, args);
                break;
                default:
                    fprintf(stderr, "output gate is invalid\n");
                    exit(1);
                break;
            }
            break;
        default:
            fprintf(stderr, "unexpected type :%d\n", node->type);
            exit(1);
    }
}

