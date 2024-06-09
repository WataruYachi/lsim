#ifndef QM_H
#define QM_H

#include "circuit.h"

table qm(table t);
table minimalizeCircuit(circuit c);
void qmsample(circuit c);

#endif
