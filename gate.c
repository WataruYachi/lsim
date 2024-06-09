#include <gc/gc.h>
#include <stdio.h>
#include <stdlib.h>
#include <gc.h>
#include "gate.h"

gate And(char *id, int argsNum, gate args[]) {
    gate g = GC_MALLOC(sizeof(struct gate));
    g -> op = AND_GATE;
    g -> id = id;
    g -> argsNum = argsNum;
    g -> args = args;
    g -> value = X;
    return g;
}

gate Or(char *id, int argsNum, gate args[]) {
    gate g = GC_MALLOC(sizeof(struct gate));
    g -> op = OR_GATE;
    g -> id = id;
    g -> argsNum = argsNum;
    g -> args = args;
    g -> value = X;
    return g;
}

gate Not(char *id, gate arg) {
    gate g = GC_MALLOC(sizeof(struct gate));
    gate *array = gateArray(1);
    array[0] = arg;
    g -> op = NOT_GATE;
    g -> id = id;
    g -> argsNum = 1;
    g -> args = array;
    g -> value = X;
    return g;
}

gate Xor(char *id, int argsNum, gate args[]) {
    gate g = GC_MALLOC(sizeof(struct gate));
    g -> op = XOR_GATE;
    g -> id = id;
    g -> argsNum = argsNum;
    g -> args = args;
    g -> value = X;
    return g;
}

gate Nand(char *id, int argsNum, gate args[]) {
    gate g = GC_MALLOC(sizeof(struct gate));
    g -> op = NAND_GATE;
    g -> id = id;
    g -> argsNum = argsNum;
    g -> args = args;
    g -> value = X;
    return g;
}

gate Nor(char *id, int argsNum, gate args[]) {
    gate g = GC_MALLOC(sizeof(struct gate));
    g -> op = NOR_GATE;
    g -> id = id;
    g -> argsNum = argsNum;
    g -> args = args;
    g -> value = X;
    return g;
}

gate Input(char *id) {
    gate g = GC_MALLOC(sizeof(struct gate));
    g -> op = INPUT_GATE;
    g -> id = id;
    g -> argsNum = 0;
    g -> args = NULL;
    g -> value = X;
    return g;
}

gate Output(char *id, gate arg) {
    gate g = GC_MALLOC(sizeof(struct gate));
    gate *array = gateArray(1);
    array[0] = arg;
    g -> op = OUTPUT_GATE;
    g -> id = id;
    g -> argsNum = 1;
    g -> args = array;
    g -> value = X;
    return g;
}

gate *gateArray(int n) {
    if (n < 0)
        return NULL;
    gate *array = GC_MALLOC(sizeof(gate) * n);
    for (int i = 0; i < n; i++) {
        array[i] = NULL;
    }
    return array;
}

gate *inputArray(int n, char **ids){
    gate *array = gateArray(n);
    for (int i = 0; i < n; i++) {
        array[i] = Input(ids[i]);
    }
    return array; 
}

void printGate(gate g) {
    char *operator;
    switch (g -> op) {
        case AND_GATE:
            operator = "AND_GATE";
            break;
        case OR_GATE:
            operator = "OR_GATE";
            break;
        case NOT_GATE:
            operator = "NOT_GATE";
            break;
        case XOR_GATE:
            operator = "XOR_GATE";
            break;
        case NAND_GATE:
            operator = "NAND_GATE";
            break;
        case NOR_GATE:
            operator = "NOR_GATE";
            break;
        case INPUT_GATE:
            operator = "INPUT_GATE";
            break;
        case OUTPUT_GATE:
            operator = "OUTPUT_GATE";
            break;
        default:
            operator = "NULL"; 
            break;
    }
    printf("%s, %s\n", g->id, operator);
    for(int i = 0; i < g->argsNum; i++) {
        printf("    %s:%d",g->id,i);
        printGate(g->args[i]);
    }
}

int evalGateValue(gate g){
    /*if (g->value != -1) {
        return g->value;
    }
    */
    int *argsValues = GC_MALLOC(sizeof(bit) * g->argsNum);
    
    for (int i = 0; i < g->argsNum; i++) {
        argsValues[i] = evalGateValue(g->args[i]);
    }

    int v;
    int t;
    int nx;
    switch (g->op) {
        case AND_GATE:
            v = T;
            nx = 0;
            for (int i = 0; i < g->argsNum; i++) {
                if (argsValues[i] == F) {
                    v = F;
                    break;
                } else if (argsValues[i] == X) {
                    nx = 1;
                }
            }
            if (v == T && nx)
                v = X;
            break;

        case OR_GATE:
            v = F;
            nx = 0;
            for (int i = 0; i < g->argsNum; i++) {
                if (argsValues[i] == T) {
                    v = T;
                    break;
                } else if (argsValues[i] == -1) {
                    nx = 1;
                }
            }
            if (v == F && nx)
                v = X; 
            break;

        case NOT_GATE:
            if (argsValues[0] == X) {
                v = X;
            } else if (argsValues[0] == T) {
                v = F;
            } else {
                v = T;
            }
            break;

        case XOR_GATE:
            int nt = 0;
            for (int i = 0; i < g->argsNum; i++) {
                if (argsValues[i] == X) {
                    v = X;
                    break;
                } else if (argsValues[i] == T) {
                    nt++;
                }
            } 
            if (nt % 2 == 0) {
                v = F;
            } else {
                v = T;
            } 
            break;

        case NAND_GATE:
            v = T;
            nx = 0;
            for (int i = 0; i < g->argsNum; i++) {
                if (argsValues[i] == F) {
                    v = F;
                    break;
                } else if (argsValues[i] == X) {
                    nx = 1;
                }
            }
            if (v == T && nx) {
                v = X;
            } else if (v == F) {
                v = T;
            } else {
                v = F;
            }
            break;

        case NOR_GATE:
            v = F;
            nx = 0;
            for (int i = 0; i < g->argsNum; i++) {
                if (argsValues[i] == T) {
                    v = T;
                    break;
                } else if (argsValues[i] == -1) {
                    nx = 1;
                }
            }
            if (v == F && nx) {
                v = X; 
            } else if (v == T) {
                v = F;
            } else {
                v = T;
            }
            break;

        case INPUT_GATE:
            return g->value;

        case OUTPUT_GATE:
            g->value = argsValues[0];
            return g->value;

        default:
            break;
    }

    g->value = v;
    return g->value;
}

void setGateValue(gate g, bit v){
    g->value = v;
}



