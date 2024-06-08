#include <gc/gc.h>
#include <stdio.h>
#include <stdlib.h>
#include "gate.h"
#include "circuit.h"
#include "truthTable.h"

int *neighbor(int n, int *a, int *b) {
    int *c = GC_MALLOC(sizeof(int) * n);
    int m = 0;
    for (int i = 0; i < n; i++) {
        if(m > 1) {
            GC_free(c);
            return NULL;
        }
        if (a[i] == b[i] && a[i] != -1 && b[i] != -1) {
            c[i] = a[i];
        } else if (a[i] == b[i] && a[i] == -1 && b[i] == -1){
            c[i] = -1;
        } else {
            c[i] = -1;
            m++;
        }
    }
    return c;
}

struct table {
    int m, n;
    int *table;
};

typedef struct table *table; 

table newTable(int m, int n) {
    table t = GC_MALLOC(sizeof(struct table));
    int *table = GC_MALLOC(sizeof(int) * m * n);
    t->m = m;
    t->n = n;
    t->table = table;
    return t;
}

void setTableValue(table t, int x, int y, int v) {
    int p = t->n * y + x;
    t->table[p] = v;
}

int getTableValue(table t, int x, int y) {
    int p = t->n * y + x;
    return t->table[p];
}

int *getTableRow(table t, int y) {
    int *r = GC_MALLOC(sizeof(int) * t->n);
    for(int i=0; i < t->n; i++) {
        r[i] = getTableValue(t, i, y);
    }
    return r;
}

void addTableRow(table t, int a) {
    t->table = GC_REALLOC(t->table, sizeof(int) * (t->m + a) * t->n);
    t->m = t->m + a;
}

void setTableRow(table t, int y, int *r) {
    for (int i = 0; i < t->n; i++) {
        setTableValue(t, i, y, r[i]);
    }
}

void printTable (table t) {
    for (int i = 0; i < t->m; i++) {
        for (int j = 0; j < t->n; j++) {
            printf("%d, ", getTableValue(t, j, i));
        }
        printf("\n");
    }
}

int countTrueNumber(int n, int *bits) {
    int c = 0;
    for (int i = 0; i < n; i++) {
        if (bits[i] == 1) {
            c++;
        }
    }
    return c;
}

table sumOfProducts(truthTable tt, int o) {
    table t = newTable(0,tt->inputNum);
    for (int i = 0; i < tt->m; i++) {
        int *r = getRow(tt, i);
        if (r[tt->inputNum + o] == 1) {
            addTableRow(t, 1);
            for (int j = 0; j < t->n; j++) {
                printf("r:%d",r[j]);
                setTableValue(t, j, t->m-1, r[j]);
                printf("t:%d",getTableValue(t, j, t->m-1));
            }
            printf("\n");
        }
    }
    return t;
}

void printArray(int n, int *a) {
    for (int i = 0; i < n; i++) {
        printf("%d, ", a[i]);
    }
    printf("\n");
}

int isAllChecked(int n, int *check) {
    for(int i = 0; i < n; i++) {
        if (check[i] != 1) {
            return 0;
        }
    }
    return 1;
}

int checkNeighbor(int **checkA, int **groupA, table t, table *ntA) {
    int m = t->m;
    int n = t-> n;
    int c = 0;
    
    int *check = GC_MALLOC(sizeof(int) * m);
    table nt;

    for (int i = 0; i < m; i++) {
        check[i] = 0;
    }

    int *group = GC_MALLOC(sizeof(int) * m);
    for (int i = 0; i < m; i++) {
        int *r = getTableRow(t, i);
        int c = countTrueNumber(n, r);
        group[i] = c;
    }

    nt = newTable(0, n);
    int *e = NULL;
    for (int i = 0; i < m; i++) {
        for (int j = i; j < m; j++) {
            if (abs(group[i] - group[j]) == 1) {
                e = neighbor(n, getTableRow(t,i), getTableRow(t,j)); 
                if (e != NULL) {
                    check[i] = 1;
                    check[j] = 1;
                    addTableRow(nt,1);
                    setTableRow(nt, nt->m - 1, e);
                    c++;
                }
            }
        }
    }
    printArray(n, check);
    *checkA = check;
    *groupA = group;
    *ntA = nt;
    return c;
}

// bがaのインスタンスかどうか
int isInstance(int n, int *a, int *b) {
    for (int i = 0; i < n; i++) {
        if (a[i] == -1 || (a[i] == b[i])) {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
}

table calcPrimeImplicant(table t) {
    table primes = newTable(0,t->n);
    int *check;
    int *group;
    int m;
    int n = t->n;
    table nt;

    // 初期化
    table tp = newTable(t->m, t->n);
    for (int i = 0; i < t->m; i++) {
        setTableRow(tp, i, getTableRow(t, i));
    }

    printf("tp:\n");
    printTable(tp);

    while (1) {
        m = tp->m;
        nt = newTable(0, n);
        check = GC_MALLOC(sizeof(int) * m);
        group = GC_MALLOC(sizeof(int) * m);
        for (int i = 0; i < m; i++) {
            check[i] = 0;
            int *r = getTableRow(tp, i);
            group[i] = countTrueNumber(n, r);
        }

        int *e = NULL;
        for (int i = 0; i < m; i++) {
            for (int j = i; j < m; j++) {
                if (abs(group[i] - group[j]) == 1) {
                    e = neighbor(n, getTableRow(tp,i), getTableRow(tp,j));
                    if (e != NULL) {
                        check[i] = 1;
                        check[j] = 1;
                        addTableRow(nt,1);
                        setTableRow(nt, nt->m - 1, e);
                    }
                }
            }
        }

        for (int i = 0; i < m; i++) {
            printf("c:[%d]%d",i,check[i]);
            if (check[i] == 0) {
                printf("bbb");
                addTableRow(primes,1);
                setTableRow(primes, primes->m - 1, getTableRow(tp, i));
            }
        }
        printf("check\n");
        printArray(m, check);
        printf("primes:\n");
        printTable(primes);

        if (nt->m == 0) {
            break;
        } else {
            tp = nt;
        }
    }

    return primes;
}

table qm(table t) {
    //truthTable tt = makeTruthTable(c);
    table sp = t;
    //table t = sumOfProducts(tt, 0);  // 1つ目の出力だけ考える。
    table nt;
    int m;
    int n = t->n;
    int *check;
    int *group;
    printTable(t);

    table primes = calcPrimeImplicant(t);
    printTable(primes);

/*
    printf("t:\n");
    printTable(t);
    printf("nt:\n");
    printTable(nt);

    // main terms
    table mtt = newTable(0, n);
    for (int i = 0; i < t->m; i++) {
        if (check[i] == 0) {
            addTableRow(mtt, 1);
            setTableRow(mtt, mtt->m -1, getTableRow(t, i));
        } 
    }

    for (int i = 0; i < nt->m; i++) {
        addTableRow(mtt, 1);
        setTableRow(mtt, mtt->m - 1, getTableRow(nt, i));
    }

    printf("mtt:\n");    
    printTable(mtt);

    // essential term
    table es = newTable(0,n);

    for (int i = 0; i < sp->m; i++) {
        int c = 0;
        int *s = getTableRow(sp, i);
        printf("s:");
        printArray(sp->n, s);
        for (int j = 0; j < mtt->m; j++) {
            printf("mtt[%d]:", j);
            printArray(n,getTableRow(mtt,j));
            if (isInstance(n, getTableRow(mtt,j), s)) {
                c++;
            }
        }
        printf("c:%d\n", c);
        if (c == 1) {
            addTableRow(es, 1);
            setTableRow(es, es->m - 1, getTableRow(mtt,i));
        }
    }

    printf("es:\n");
    printTable(es);

*/
    return NULL;
}

void qmsample(circuit c) {
    truthTable tt = makeTruthTable(c);
    table t = sumOfProducts(tt, 0);
    qm(t);

}
