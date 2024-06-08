#ifndef TABLE_H
#define TABLE_H

#include <gc/gc.h>
#include <gc.h>
#include <stdint.h>

// 1要素2bit
// 00 = 0
// 01 = 1
// 11 = X

typedef enum bit {
    F = 0,
    T = 1,
    X = 3
} bit;

struct row {
    uint64_t r[4];
};

typedef struct row *row;

struct table {
    unsigned int n;
    unsigned int m;
    struct row *rows;
};

typedef struct table *table;

row Row();
table Table(int n, int m);
void addEmptyRow(int a);
void addRow(table t, row r);
void setRowValue(row r, int n, bit v);
void setTableValue(table t, int x, int y, bit v);



#endif
