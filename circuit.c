#include <gc/gc.h>
#include <stdio.h>
#include <stdlib.h>
#include <gc.h>
#include "gate.h"
#include "circuit.h"
#include "table.h"
#include "bit.h"
//#include "truthTable.h"
#include <math.h>

void succ(int, bitArray);

circuit Circuit(char *id, int inputNum, gate *input, int outputNum, gate *output) {
    circuit c = GC_MALLOC(sizeof(struct circuit));
    c->id = id;
    c->inputNum = inputNum;
    c->input = input;
    c->outputNum = outputNum;
    c->output = output;
    return c;
}

bitArray runCircuitWithInput(circuit c, bitArray input){
    for (int i = 0; i < c->inputNum; i++) {
        setGateValue(c->input[i], getBit(input, i));
    }

    bitArray output = BitArray();

    for (int i = 0; i < c->outputNum; i++) {
        bit v = evalGateValue(c->output[i]);
        setBit(output, i, v); //evalGateValue(c->output[i]);
    }

    return output;
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

    bitArray input = BitArray();
    for (int i = 0; i < c->inputNum; i++) {
        setBit(input, i, F);
    }

    bitArray output;
    for (int i = 0; i < tt->input->m; i++) {
        output = runCircuitWithInput(c, input);
        setInputRow(tt, i, input);
        setOutputRow(tt, i, output);
        succ(c->inputNum, input);
    }

    return tt;
}

void succSub(int n, int p, bitArray bs) {
    if (p > n) return;
    if (getBit(bs, n-p) == T) {
        setBit(bs, n-p, F);
        succSub(n, p+1, bs);
    } else {
        setBit(bs, n-p, T);
        return;
    }
}

void succ(int n, bitArray bs) {
    succSub(n, 1, bs);
}


void printCircuit(circuit c) {
    printf("circuit:");
    printf("%s\n", c->id);
    printf("input\n");
    for (int i = 0; i < c->inputNum; i++) {
        printGate(c->input[i]);
    }
    printf("output\n");
    for (int i = 0; i < c->outputNum; i++) {
        printGate(c->output[i]);
    }
}



