#include <gc/gc.h>
#include <stdio.h>
#include <stdlib.h>
#include <gc.h>
#include "gate.h"
#include "circuit.h"
#include "truthTable.h"
#include <math.h>

void combination(int,int,int,int**);
void succ(int, int *);

circuit Circuit(char *id, int inputNum, gate *input, int outputNum, gate *output) {
    circuit c = GC_MALLOC(sizeof(struct circuit));
    c->inputNum = inputNum;
    c->input = input;
    c->outputNum = outputNum;
    c->output = output;
    return c;
}

int *runCircuitWithInput(circuit c, int inputNum, int *input){
    for (int i = 0; i < inputNum; i++) {
        setGateValue(c->input[i], input[i]);
    }

    int *results = GC_MALLOC(sizeof(int) * (c->inputNum + c->outputNum));
    for (int i = 0; i < c->inputNum; i++) {
        results[i] = input[i];
    }
    for (int i = 0; i < c->outputNum; i++) {
        int r = evalGateValue(c->output[i]);
        results[c->inputNum + i] = r; //evalGateValue(c->output[i]);
    }

    return results;
}

void printResult(int n, result *results) {
    for (int i = 0; i < n; i++) {
        printf("%s:%d ", results[i].id, results[i].v);
    }
    printf("\n");
}

truthTable makeTruthTable(circuit c) { 
    char **inputVars = GC_MALLOC(sizeof(char *) * c->inputNum);
    char **outputVars = GC_MALLOC(sizeof(char *) * c->outputNum);

    for (int i = 0; i < c->inputNum; i++) {
        inputVars[i] = c->input[i]->id;
    }

    for (int i = 0; i < c->outputNum; i++) {
        outputVars[i] = c->output[i]->id;
    }

    truthTable tt = TruthTable(c->inputNum, inputVars, c->outputNum, outputVars);

    int *input = GC_MALLOC(sizeof(int) * c->inputNum);
    for (int i = 0; i < c->inputNum; i++) {
        input[i] = 0;
    }
    int *row;

    for (int i = 0; i < tt->m; i++) {
        row = runCircuitWithInput(c, c->inputNum, input);
        setRow(tt, i, row);
        succ(c->inputNum, input);
    }

    return tt;
}

void succSub(int n, int p, int *bits) {
    if (p > n) return;
    if (bits[n-p] == 1) {
        bits[n-p] = 0;
        succSub(n, p+1, bits);
    } else {
        bits[n-p] = 1;
        return;
    }
}

void succ(int n, int *bits) {
    succSub(n, 1, bits);
}

