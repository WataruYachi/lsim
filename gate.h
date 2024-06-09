#ifndef GATE_H
#define GATE_H

#include "bit.h"

typedef enum {AND_GATE, OR_GATE, NOT_GATE,
    XOR_GATE, NAND_GATE, NOR_GATE, INPUT_GATE, OUTPUT_GATE} operator;

struct gate {
    operator op;
    char *id;
    int argsNum;
    struct gate **args;
    bit value;
};

typedef struct gate *gate;

gate And   (char *, int, gate[]);
gate Or    (char *, int, gate[]);
gate Not   (char *, gate);
gate Xor   (char *, int, gate[]);
gate Nand  (char *, int, gate[]);
gate Nor   (char *, int, gate[]);
gate Input (char *);
gate Output(char *, gate);

void printGate(gate);
gate *gateArray(int);
gate *inputArray(int, char **);
int evalGateValue(gate);
void setGateValue(gate, bit);

#endif

