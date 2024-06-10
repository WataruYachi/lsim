#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "gate.h"
#include "table.h"

struct circuit {
    char *id;
    int inputNum;
    gate *input;
    int outputNum;
    gate *output;
};

typedef struct circuit *circuit;

circuit Circuit(char *, int, gate[], int, gate[]);
bitArray runCircuitWithInput(circuit c, bitArray input);
truthTable makeTruthTable(circuit);
void printCircuit(circuit c);
#endif
