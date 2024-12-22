#ifndef TABLE_H
#define TABLE_H

#include <gc/gc.h>
#include <gc.h>
#include <stdint.h>

#include "bit.h"

struct table {
    unsigned int n;
    unsigned int m;
    bitArray rows;
};

typedef struct table *table;

table Table(int m, int n);
void addEmptyRow(int a);
void addTableRow(table t, bitArray r);
void setTableRow(table t, int y, bitArray r);
bitArray getTableRow(table t, int y);
void setTableValue(table t, int x, int y, bit v);
table appendTable(table, table);
void printTable(table t);
table copyTable(table t);

struct truthTable {
    unsigned int inputNum;
    unsigned int outputNum;
    char **inputVars;
    char **outputVars;
    table input;
    table output;
};

typedef struct truthTable *truthTable;

truthTable TruthTable(unsigned int inputNum, char *inputVars[],
        unsigned int outputNum, char *outputVars[]);

void setInput(truthTable tt, int x, int y, bit v);
void setOutput(truthTable tt, int x, int y, bit v);
void setInputRow(truthTable tt, int y, bitArray r);
void setOutputRow(truthTable tt, int y, bitArray r);
bitArray getInputRow(truthTable tt, int y);
bitArray getOutputRow(truthTable tt, int y);
void printTruthTable(truthTable tt);


#endif
