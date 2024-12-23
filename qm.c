#include <gc/gc.h>
#include <stdio.h>
#include <stdlib.h>
#include "gate.h"
#include "circuit.h"
#include "table.h"
#include "bit.h"

table sumOfProducts(truthTable, int);
bitArray neighbor(int, bitArray, bitArray);
table checkNeighborGroup(int i, table *, table *); 
int countTrueNumber(int, bitArray);
table loop(table t, table *ps);
table nubTable(table t);
int elem(bitArray bs, table t);
int isInstance(int n, bitArray a, bitArray b);
bitArray simpleOne(table t);
int isSimple(int n, bitArray as, bitArray bs);

/* 加法標準形の回路を受け取り、QM法を用いて簡単化する */
table qm(table t) {
    table rs = Table(0, t->n);
    table ps = Table(0, t->n);

    // STEP3 - STEP4 のループ
    table next = loop(t, &ps);
    while (!next->m == 0) {
        next = loop(next, &ps);
    }
    // 主項のテーブル
    ps = nubTable(ps);

    int *ignoreRow = GC_MALLOC(sizeof(int) * ps->m);
    int *ignoreCol = GC_MALLOC(sizeof(int) * t->m);
    // 主項を行、最小項を列にとった表
    // 1 = >, 2 = ◎, 3 = ○ 
    int **checkTable = GC_MALLOC(sizeof(int *) * t->m);
    for (int i = 0; i < t->m; i++) {
        checkTable[i] = GC_MALLOC(sizeof(int) * ps->m);
        ignoreCol[i] = 0;
        for (int j = 0; j < ps->m; j++) {
            checkTable[i][j] = 0;
            ignoreRow[j] = 0;
        }
    }
    
    // 表に>をつける
    for (int i = 0; i < t->m; i++) {
        bitArray bs = getTableRow(t, i);
        for (int j = 0; j < ps->m; j++) {
            if (isInstance(t->n, getTableRow(ps, j), bs)) {
                checkTable[i][j] = 1;
            }
        }
    }

    // 表に◎をつける
    for (int i = 0; i < t->m; i++) {
        int p = 0;
        int sum = 0;
        for (int j = 0; j < ps->m; j++) {
            sum += checkTable[i][j];
            if (checkTable[i][j] == 1) {
                p = j;
            }
        }
        if (sum == 1) {
            checkTable[i][p] = 2;
            ignoreRow[p] = 1;
            ignoreCol[i] = 1;
            addTableRow(rs, getTableRow(ps, p));
            // ○をつける
            for (int ii = 0; ii < t->m; ii++) {
                if(checkTable[ii][p] == 1 && !i == ii) {
                    checkTable[ii][p] = 3;
                }
            }
        }
    }

    // 簡単なものの組合せを探す
    for (int i = 0; i < t->m; i++) {
        table tmp = Table(0,t->n);
        for (int j = 0; j < ps->m; j++) {
            if(ignoreCol[i] == 0 && ignoreRow[j] == 0) {
                addTableRow(tmp, getTableRow(ps,j));
            }
        }
        if (tmp->m > 0) {
            addTableRow(rs,simpleOne(tmp));
        }
    }

    rs = nubTable(rs);

    return rs;
}

table loop(table t, table *ps) {
    int m = t->m;
    int n = t->n;

    table next = Table(0,n);
    // STEP1 グループ分け
    // group[i] = n はtable[i]の行がグループn に属していることを表す
    int *group = GC_MALLOC(sizeof(int) * m);

    for (int i = 0; i < m; i++) {
        group[i] = countTrueNumber(n, getTableRow(t,i));
    }
    
    int *check = GC_MALLOC(sizeof(int) * m);
    // 1 は $ がついていることを、0 は印無し
    for (int i = 0; i < m; i++) {
        check[i] = 0;
    }

    for (int i = 0; i < m; i++) {
        int gi = group[i];
        bitArray bi = getTableRow(t, i);
        for (int j = 0; j < m; j++) {
            int gj = group[j];
            if (gi == gj+1) {
                bitArray bs = neighbor(n, bi, getTableRow(t, j));
                if (bs == NULL) {
                    continue;
                } else {
                    addTableRow(next, bs);
                    check[i] = 1;
                    check[j] = 1;
                }
            } else {
                continue;
            }
        }
    }

    for (int i = 0; i < m; i++) {
        if (check[i] == 0) {
            addTableRow(*ps, getTableRow(t, i));
        }
    }
    return next;
}

bitArray simpleOne(table t) {
    if (t->n == 0) return NULL;

    bitArray bs = getTableRow(t,0);
    for(int i = 1; i < t->m; i++) {
        bitArray tmp = getTableRow(t,i);
        if(!isSimple(t->n, bs, tmp)) {
            bs = getTableRow(t,i);
        }
    }

    return bs;
}

int numOfX(int n, bitArray bs) {
    int c = 0;
    for (int i = 0; i < n; i++) {
        if(getBit(bs, i) == X) {
            c++;
        }
    }
    return c;
}

int isSimple(int n, bitArray as, bitArray bs) {
    int ax = numOfX(n, as);
    int bx = numOfX(n, bs);
    if (ax > bx) {
        return 1;
    } else {
        return 0;
    }
}
// 重複する要素を削除する
table nubTable(table t) {
    int m = t->m;
    int n = t->n;
    table s = Table(0,t->n);

    for (int i = 0; i < m; i++) {
        bitArray bs = getTableRow(t, i);
        if (elem(bs, s)) {
            continue;
        } else {
            addTableRow(s, bs);
        }
    }
    return s;
}

int elem(bitArray bs, table t) {
    for (int i = 0; i < t->m; i++) {
        if (bitArrayEq(bs, getTableRow(t,i))) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

/* 
２つのビット列が論理的隣接性を持つかどうかを判定する
持つ場合は隣接性のあるビット位置を X で置き換えたビット列を返す
そうでない場合は、NULLを返す
*/
bitArray neighbor(int n, bitArray a, bitArray b) {
    bitArray c = BitArray();
    int dn = 0;
    for (int i = 0; i < n; i++) {
        bit va = getBit(a, i);
        bit vb = getBit(b, i);
        if (va == vb) {
            setBit(c, i, va);
        } else if (!(va == vb) && !(va == X) && !(vb == X)){
            setBit(c, i, X);
            dn++;
        } else {
            dn++;
        }

        if(dn > 1) {
            return NULL;
        }
    }
    return c;
}

// bがaのインスタンスかどうか
int isInstance(int n, bitArray a, bitArray b) {
    //printf("check instance\n");
    //printBitArray(n, a);
    //printf("\n");
    //printBitArray(n, b);
    //printf("\n");
    for (int i = 0; i < n; i++) {
        bit va = getBit(a, i);
        bit vb = getBit(b, i);
        if (va == X || (va == vb)) {
            continue;
        } else {
            return 0;
        }
    }
    //printf("true\n");
    return 1;
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



