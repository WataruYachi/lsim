#include <gc/gc.h>
#include <stdio.h>
#include <stdlib.h>
#include "gate.h"
#include "circuit.h"
#include "table.h"
#include "bit.h"

bitArray neighbor(int n, bitArray a, bitArray b) {
    bitArray c = BitArray();
    int tm = 0;
    for (int i = 0; i < n; i++) {
        if(tm > 1) {
            return NULL;
        }
        bit va = getBit(a, i);
        bit vb = getBit(b, i);
        if (va == vb && va != X && vb != X) {
            setBit(c, i, va);
        } else if (va == vb && va == X && vb == X){
            setBit(c, i, X);
        } else {
            setBit(c, i, X);
            tm++;
        }
    }
    return c;
}

int countTrueNumber(int n, bitArray bs) {
    int c = 0;
    for (int i = 0; i < n; i++) {
        if (getBit(bs, i) == T) {
            c++;
        }
    }
    return c;
}

table sumOfProducts(truthTable tt, int o) {
    table t = Table(0, tt->inputNum);
    for (int i = 0; i < tt->input->m; i++) {
        if (getBit(getOutputRow(tt,i), o) == T) {
            addTableRow(t, getInputRow(tt, i));
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
        bitArray bs = getTableRow(t, i);
        int c = countTrueNumber(n, bs);
        group[i] = c;
    }

    nt = Table(0, n);
    bitArray e = NULL;
    for (int i = 0; i < m; i++) {
        for (int j = i; j < m; j++) {
            if (abs(group[i] - group[j]) == 1) {
                e = neighbor(n, getTableRow(t,i), getTableRow(t,j)); 
                if (e != NULL) {
                    check[i] = 1;
                    check[j] = 1;
                    addTableRow(nt, e);
                    c++;
                }
            }
        }
    }
    *checkA = check;
    *groupA = group;
    *ntA = nt;
    return c;
}

// bがaのインスタンスかどうか
int isInstance(int n, bitArray a, bitArray b) {
    for (int i = 0; i < n; i++) {
        bit va = getBit(a, i);
        bit vb = getBit(b, i);
        if (va == X || (va == vb)) {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
}

int elem(table t, bitArray bs) {
    for (int i = 0; i < t->m; i++) {
        if (bitArrayEq(getTableRow(t, i), bs)) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

table calcPrimeImplicant(table t) {
    table primes = Table(0,t->n);
    int *check;
    int *group;
    int m;
    int n = t->n;
    table nt;

    // 初期化
    table tp = Table(t->m, t->n);
    for (int i = 0; i < t->m; i++) {
        setTableRow(tp, i, getTableRow(t, i));
    }

    while (1) {
        m = tp->m;
        nt = Table(0, n);
        check = GC_MALLOC(sizeof(int) * m);
        group = GC_MALLOC(sizeof(int) * m);
        for (int i = 0; i < m; i++) {
            check[i] = 0;
            bitArray bs = getTableRow(tp, i);
            group[i] = countTrueNumber(n, bs);
        }

        bitArray e = NULL;
        for (int i = 0; i < m; i++) {
            for (int j = i; j < m; j++) {
                if (abs(group[i] - group[j]) == 1) {
                    e = neighbor(n, getTableRow(tp,i), getTableRow(tp,j));
                    if (e != NULL) {
                        check[i] = 1;
                        check[j] = 1;
                        addTableRow(nt, e);
                    }
                }
            }
        }

        for (int i = 0; i < m; i++) {
            if (check[i] == 0 && !elem(primes, getTableRow(tp, i))) {
                addTableRow(primes, getTableRow(tp, i));
            }
        }

        if (nt->m == 0) {
            break;
        } else {
            tp = nt;
        }
    }

    return primes;
}



table calcEssentialTerms(table ps, table ms, int ***ct) {
    table es = Table(0, ps->n);
    int n = ps->n;
    int *essentialExists = GC_MALLOC(sizeof(int) * ps->m);

    // 1: >
    // 2: ◎
    // 3: ○
    int **checkTable = GC_MALLOC(sizeof(int*) * ps->m);
    for (int i = 0; i < ps->m; i++) {
        checkTable[i] = GC_MALLOC(sizeof(int) * ms->m);
        essentialExists[i] = 0;
    }

    
    int c;
    int index;

    for (int i = 0; i < ms->m; i++) {
        c = 0;
        index = 0;
        for (int j = 0; j < ps->m; j++) {
            bitArray mt = getTableRow(ms, i);
            if (isInstance(n, getTableRow(ps, j), mt)) {
                checkTable[j][i] = 1;
                index = j;
                c++;
            }
        }

        if (c == 1) {
            bitArray bs = getTableRow(ps, index);
            if (!elem(es, bs)) { 
                addTableRow(es, bs);
            }
            checkTable[index][i] = 2;
            essentialExists[index] = 1;
        }
    }

    for (int i = 0; i < ps->m; i++) {
        for (int j = 0; j < ms->m; j++) {
            if (checkTable[i][j] == 1 && essentialExists[i] == 1) {
                checkTable[i][j] = 3;
            }
        }
    }

    *ct = checkTable; 
    return es;
}

int *countXF(int n, bitArray bs) {
    int *c = GC_MALLOC(sizeof(int)*2);
    c[0] = 0;
    c[1] = 0;
    for (int i = 0; i < n; i++) {
        if (getBit(bs, i) == X) {
            c[0]++;
        } else if (getBit(bs,i) == F) {
            c[1]++;
        }
    }
    return c;
}

bitArray mostSimpleTerm(table t) {
    bitArray bs = getTableRow(t, 0);
    int *c = countXF(t->n, bs);
    int *cn;
    for (int i = 1; i < t->m; i++) {
        cn = countXF(t->n, getTableRow(t,i));
        if (cn[0] < c[0]) {
            bs = getTableRow(t, i);
        } else if (cn[0] == c[0] && cn[1] < c[0]) {
            bs = getTableRow(t, i);
        }
    }
    return bs;
}

table qm(table ms) {
    //truthTable tt = makeTruthTable(c);
    table sp = ms;
    //table t = sumOfProducts(tt, 0);  // 1つ目の出力だけ考える。
    table nt;
    int m;
    int n = ms->n;
    int *check;
    int *group;

    table ps = calcPrimeImplicant(ms);

    int **checkTable;
    table es = calcEssentialTerms(ps, ms, &checkTable);

    int *ignoreRow = GC_MALLOC(sizeof(int) * ps->m);
    int *ignoreCol = GC_MALLOC(sizeof(int) * ms->m);

    for (int i = 0; i < ps->m; i++) {
        //ignoreRow[i] = 0;
        for (int j = 0; j < ms->m; j++) {
            //ignoreCol[j] = 0;
            if (checkTable[i][j] == 2 || checkTable[i][j] == 3) {
                ignoreRow[i] = 1;
                ignoreCol[j] = 1;
            }
        }
    }

    // 残った主項を格納するテーブル
    table *rs = GC_MALLOC(sizeof(table) * ms->m);
    int *rscheck = GC_MALLOC(sizeof(int) * ms->m);
    for (int i = 0; i < ms->m; i++) {
        rs[i] = Table(0, ms->n);
        rscheck[i] = 0;
    }

    for (int i = 0; i < ps->m; i++) {
        for (int j = 0; j < ms->m; j++) {
            if (checkTable[i][j] == 1 && ignoreRow[i] == 0 && ignoreCol[j] == 0) {
                addTableRow(rs[j], getTableRow(ps, i));
                rscheck[j] = 1;
            }
        }
    }

    for (int i = 0; i < ms->m; i++) {
        if (rscheck[i] == 1) {
            bitArray newr = mostSimpleTerm(rs[i]);
            if (!elem(es, newr)) {
                addTableRow(es, newr);
            }
        }
    }

    return es;
}

table sampleTable() {
    table t = Table(7, 4);
    bit bt[7][4] = {{F,F,F,T},{F,T,F,T},{F,T,T,F},{T,F,F,T},{F,T,T,T},{T,T,F,T},{T,T,T,F}};
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 4; j++) {
            setTableValue(t, j, i, bt[i][j]);
        }
    }
    return t;
}

circuit tableToCircuit(table t, char **inputVars, char *outputVar) {
    gate *input = inputArray(t->n, inputVars);
    gate *andGates = gateArray(t->m);
    for (int i = 0; i < t->m; i++) {
        bitArray bs = getTableRow(t, i);
        int *cx = countXF(t->n, bs);
        int n = t->n - cx[0];
        gate *andinput = gateArray(n);
        for (int j = 0; j < t->n; j++) {
            switch (getBit(bs, j)) {
                case F:
                    andinput[j] = Not("not", input[j]);
                    break;
                case T:
                    andinput[j] = input[j];
                    break;
                case X:
                    break;
            }
        }
        andGates[i] = And("and", n, andinput);
    }
    gate or = Or("or", t->m, andGates);
    gate *output = gateArray(1);
    output[0] = Output("output", or);
    return Circuit("new circuit", t->m, input, 1, output); 
}

void minimalizeCircuit(circuit c) {
    truthTable tt = makeTruthTable(c);
    table t = sumOfProducts(tt, 0);
    table mc = qm(t);
    printf("minimalized circuit\n");
    for (int i = 0; i < tt->inputNum; i++) {
        printf("%s,", tt->inputVars[i]);
    }
    printf("\n");
    printTable(mc);

    //printf("truth table of after minimalizing\n");
    //circuit nc = tableToCircuit(mc, tt->inputVars, tt->outputVars[0]);
    //truthTable tt2 = makeTruthTable(nc);
    //printTruthTable(tt2); 
}

