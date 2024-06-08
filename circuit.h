#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "gate.h"
#include "truthTable.h"

typedef struct {
    char *id;
    int v;
} result;

struct circuit {
    char *id;
    int inputNum;
    gate *input;
    int outputNum;
    gate *output;
};



typedef struct circuit *circuit;

circuit Circuit(char *, int, gate[], int, gate[]);
result *resultArray(int n);
int *runCircuitWithInput(circuit c, int inputNum, int *input);
void printResult(int, result *);
void succ(int, int*);
truthTable makeTruthTable(circuit);
//void printTruthTable(truthTable);


#endif
