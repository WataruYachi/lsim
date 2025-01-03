#include <gc/gc.h>
#include <gc.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "table.h"
#include "bit.h"

table Table(int m, int n) {
    table t = GC_MALLOC(sizeof(struct table));
    t->m = m;
    t->n = n;
    t->rows = GC_MALLOC(sizeof(bitArray) * m);
    return t;
}

void setTableValue(table t, int x, int y, bit v) {
    setBit(t->rows[y], x, v);
}

bit getTableValue(table t, int x, int y) {
    return getBit(t->rows[y], x);
}

bitArray getTableRow(table t, int y) {
    return copyBitArray(t->rows[y]);
}

void addTableRow(table t, bitArray r) {
    t->rows = GC_REALLOC(t->rows, sizeof(bitArray) * t->m + 1);
    t->rows[t->m] = copyBitArray(r);
    t->m = t->m + 1;
}

table appendTable(table x, table y) {
    int n = x->n;
    //printf("x->m:%d, y->m:%d\n", x->m, y->m);
    table t = Table(0, n);
    //printTable(t);
    for (int i = 0; i < x->m; i++) {
        addTableRow(t, getTableRow(x, i));
    }
    for (int i = 0; i < y->m; i++) {
        addTableRow(t, getTableRow(y, i));
    }
    return t;
}

void setTableRow(table t, int y, bitArray r) {
    t->rows[y] = copyBitArray(r);
}

void printTable(table t) {
    //printf("m: %d, n: %d\n", t->m, t->n);
    for (int i = 0; i < t->m; i++) {
        printBitArray(t->n, t->rows[i]);
        printf("\n");
    }
}

table copyTable(table t) {
    table new = Table(0, t->n);
    for (int i = 0; i < t->m; i++) {
        addTableRow(new, getTableRow(t,i));
    }
    return new;
}


truthTable TruthTable(unsigned int inputNum, char *inputVars[],
                        unsigned int outputNum, char *outputVars[]) {
    truthTable tt = GC_MALLOC(sizeof(struct truthTable));
    int m = pow(2, inputNum);
    tt->inputNum = inputNum;
    tt->inputVars = inputVars;
    tt->outputNum = outputNum;
    tt->outputVars = outputVars;
    tt->input = Table(m, inputNum);
    tt->output = Table(m, outputNum);
    return tt;
}

void setInputRow(truthTable tt, int y, bitArray r) {
    setTableRow(tt->input, y, r);
}

void setOutputRow(truthTable tt, int y, bitArray r) {
    setTableRow(tt->output, y, r);
}

bitArray getInputRow(truthTable tt, int y) {
    return getTableRow(tt->input, y);
}

bitArray getOutputRow(truthTable tt, int y) {
    return getTableRow(tt->output, y);
}

void printTruthTable(truthTable tt) {
    for(int i = 0; i < tt->inputNum; i++) {
        printf("%s,", tt->inputVars[i]);
    }
    for (int i = 0; i < tt->outputNum; i++) {
        printf("%s,", tt->outputVars[i]);
    }
    printf("\n");

    for(int i = 0; i < tt->input->m; i++) {
        printBitArray(tt->input->n, getTableRow(tt->input, i));
        printBitArray(tt->output->n, getTableRow(tt->output, i));
        printf("\n");
    }
}

/*
int main () {
    table t = Table(2,3);
    setTableValue(t, 0, 0, F);
    setTableValue(t, 1, 0, T);
    setTableValue(t, 2, 0, X);
    setTableValue(t, 0, 1, X);
    setTableValue(t, 1, 1, X);
    setTableValue(t, 2, 1, X);
    table t2 = Table(2,1);
    setTableValue(t2, 0, 0, F);
    setTableValue(t2, 0, 1, F);
    printTable(t);
    printf("\n");
    printTable(t2);

    return 0;
}
*/
