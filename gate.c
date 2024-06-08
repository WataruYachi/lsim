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
    g -> value = -1;
    return g;
}

gate Or(char *id, int argsNum, gate args[]) {
    gate g = GC_MALLOC(sizeof(struct gate));
    g -> op = OR_GATE;
    g -> id = id;
    g -> argsNum = argsNum;
    g -> args = args;
    g -> value = -1;
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
    g -> value = -1;
    return g;
}

gate Input(char *id) {
    gate g = GC_MALLOC(sizeof(struct gate));
    g -> op = INPUT_GATE;
    g -> id = id;
    g -> argsNum = 0;
    g -> args = NULL;
    g -> value = -1;
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
    g -> value = -1;
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
    int *argsValues = GC_MALLOC(sizeof(int) * g->argsNum);
    
    for (int i = 0; i < g->argsNum; i++) {
        argsValues[i] = evalGateValue(g->args[i]);
    }

    int v;
    int t;
    int nx;
    switch (g->op) {
        case AND_GATE:
            v = 1;
            nx = 0;
            for (int i = 0; i < g->argsNum; i++) {
                if (argsValues[i] == 0) {
                    v = 0;
                    break;
                } else if (argsValues[i] == -1) {
                    nx = 1;
                }
            }
            if (v && nx)
                v = -1;
            break;

        case OR_GATE:
            v = 0;
            nx = 0;
            for (int i = 0; i < g->argsNum; i++) {
                if (argsValues[i] == 1) {
                    v = 1;
                    break;
                } else if (argsValues[i] == -1) {
                    nx = 1;
                }
            }
            if (!v && nx)
                v = -1; 
            break;

        case NOT_GATE:
            if (argsValues[0] == -1) {
                v = -1;
            } else {
                v = 1 - argsValues[0];
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

void setGateValue(gate g, int v){
    g->value = v;
}



