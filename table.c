#include <gc/gc.h>
#include <gc.h>
#include <stdio.h>
#include <stdint.h>
#include "table.h"

#define INT_INDEX(i) (i / 32)
#define BIT_INDEX(i) ((i % 32) * 2)

row Row() {
    return GC_MALLOC(sizeof(struct row));
}

table Table(int n, int m) {
    table t = GC_MALLOC(sizeof(struct table));
    t->n = n;
    t->m = m;
    t->rows = GC_MALLOC(sizeof(struct row) * m);
    return t;
}

void setRowValue(row r, int i, bit v) {
    uint64_t v64 = (uint64_t)v;
    r->r[INT_INDEX(i)] &= ~(3 << BIT_INDEX(i));
    r->r[INT_INDEX(i)] |= (v64 << BIT_INDEX(i));
}

void setTableValue(table t, int x, int y, bit v) {
    setRowValue(&t->rows[y], x, v);
}

bit getRowValue(row r, int i) {
    int bi = BIT_INDEX(i);
    int ii = r->r[INT_INDEX(i)];
    int ui = ii & (1 << bi);
    int li = ii & (1 << (bi+1));
    //printf("ui:%d,li:%d,uili:%d\n", ui,li,(ui | li) >> bi);
    return (ui | li) >> bi;
}

bit getTableValue(table t, int x, int y) {
    return getRowValue(&t->rows[y], x);
}

void addRow(table t, row r) {
    GC_REALLOC(t->rows, sizeof(struct row) * t->m + 1);
    for (int i; i < 4; i++) {
        t->rows[t->m + 1].r[i] = r->r[i]; 
    }
    t->m = t->m + 1;
}

void printRow(int n, row r) {
    for (int i = 0; i < n; i++) {
        printf("%d,", getRowValue(r, i));
    }
    printf("\n");
}

void printTable(table t) {
    for (int i = 0; i < t->m; i++) {
        printRow(t->n, &t->rows[i]);
    }
}

int main () {
    table t = Table(3,2);
    setTableValue(t, 0, 0, F);
    setTableValue(t, 1, 0, T);
    setTableValue(t, 2, 0, X);
    setTableValue(t, 0, 1, X);
    setTableValue(t, 1, 1, X);
    setTableValue(t, 2, 1, X);

    printTable(t);

    return 0;
}


