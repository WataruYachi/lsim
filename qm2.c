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
table* groupBy1(table, int *);
int countTrueNumber(int, bitArray);

/* 加法標準形の回路を受け取り、QM法を用いて簡単化する */
table qm(table ms) {

    // STEP 2
    // グループ化する
    // group[i] は 列中に1をi個持つビット列のグループを表す
    int gm = 0;
    table *group = groupBy1(ms, &gm);
    table nt = Table(0, ms->n);

    // STEP 3
    // 隣接するグループ同士から論理的隣接性を持つビット列を探し出し
    // 新しいビット列を生成する
    int **checkList = GC_MALLOC(sizeof(int *) * gm);
    for (int i = 0; i < gm; i++) {
        checkList[i] = GC_MALLOC(sizeof(int) * group[i]->m);
    }

    return NULL;
}

// STEP 1 - STEP 4のループ
// $ がつかない主項のテーブル ps と、次のループに使うテーブル tn を返す。
table loop(table t, table *ps) {
    int m = t->m;
    int n = t->n;
    table tn = Table(0, n);
    printf("loop\n");
    printTable(t);
    
    int gm = 0;
    table *group = groupBy1(t, &gm);
    for (int i = 0; i < gm; i++) {
        printf("group[%d]\n", i); 
        printTable(group[i]);
    }
    printf("grouped, gm=%d\n", gm);
    
    for (int i = 0; i < gm - 1; i++) {
        tn = appendTable(tn, checkNeighborGroup(i, group, ps));
    }

    printf("loop done\n");
    return tn;
}


// group[i]をgroup[i+1]の組み合わせを考える
table checkNeighborGroup(int i, table *group, table *checked) {
    table gi = group[i];
    table gj = group[i+1];

    int ni = gi->n;
    int mi = gi->m;
    int nj = gj->n;
    int mj = gj->m;

    printf("cng i:%d,ni:%d,mi:%d,nj:%d,mj:%d\n",i,ni,mi,nj,mj);

    table c = Table(0,ni);

    for (int h = 0; h < mi; h++) {
        for (int k = 0; k < mj; k++) {
            bitArray cb = neighbor(ni, getTableRow(gi, h), getTableRow(gj, k));
            if (cb == NULL) {
                continue;
            } else {
                addTableRow(c, cb);
                addTableRow(*checked, getTableRow(gi,h));
                addTableRow(*checked, getTableRow(gj,k));
            }
        }
    }
    printTable(c);
    printf("cng done\n");
    return c;
}

/* 
２つのビット列が論理的隣接性を持つかどうかを判定する
持つ場合は隣接性のあるビット位置を X で置き換えたビット列を返す
そうでない場合は、NULLを返す
*/
/*bitArray negihbor(int n, bitArray a, bitArray b) {
    bitArray c = BitArray();
    // 異なるビットの数
    int dn = 0; 

    for (int i = 0; i < n; i++) {
        if (dn > 1) {
            return NULL;
        }
        bit ai = getBit(a, i);
        bit bi = getBit(b, i);
        if (ai == bi) {
            setBit(c, i, ai);
        } else if (ai != bi && ai != X && bi != X) {
            setBit(c, i, X);
            dn++;
        } else {
            return NULL;
        } 
    }

    return c;
}*/

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

table* groupBy1(table t, int *maxN) {
    int m = t->m;
    int n = t->n;
    int *numberOfTrue = GC_MALLOC(sizeof(int) * m);
    int maxNum = 0;

    for (int i = 0; i < m; i++) {
        int ni = countTrueNumber(n, getTableRow(t, i));
        numberOfTrue[i] = ni;
        if (ni > maxNum) {
            maxNum = ni;
        }
    }
    
    table *group = GC_MALLOC(sizeof(table) * (maxNum + 1));
    for (int i = 0; i < (maxNum + 1); i++) {
        group[i] = Table(0,n);
    }

    for (int i = 0; i < m; i++) {
        printf("in groupBy1\n");
        printBitArray(n,getTableRow(t,i));
        printf("\n");
        addTableRow(group[numberOfTrue[i]], getTableRow(t, i));
        printf("group[%d]\n", numberOfTrue[i]);
        printTable(group[numberOfTrue[i]]);
    }

    for (int i = 0; i < maxNum + 1; i++) {
        printTable(group[i]);
    }

    *maxN = maxNum + 1; 
    return group;
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

/*
table sampleTable() {
    table t = Table(0, 4);
    bitArray a1 = BitArray();
    bitArray a2 = BitArray();
    bitArray a3 = BitArray();
    bitArray a4 = BitArray();
    bitArray a5 = BitArray();
    bitArray a6 = BitArray();
    bitArray a7 = BitArray();
    setBit(a1,0,0);
    setBit(a1,1,0);
    setBit(a1,2,0);
    setBit(a1,3,1);

    setBit(a2,0,0);
    setBit(a2,1,1);
    setBit(a2,2,0);
    setBit(a2,3,1);
    
    setBit(a3,0,0);
    setBit(a3,1,1);
    setBit(a3,2,1);
    setBit(a3,3,0);
    
    setBit(a4,0,1);
    setBit(a4,1,0);
    setBit(a4,2,0);
    setBit(a4,3,1);
    
    setBit(a5,0,0);
    setBit(a5,1,1);
    setBit(a5,2,1);
    setBit(a5,3,1);
    
    setBit(a6,0,1);
    setBit(a6,1,1);
    setBit(a6,2,0);
    setBit(a6,3,1);
    
    setBit(a7,0,1);
    setBit(a7,1,1);
    setBit(a7,2,1);
    setBit(a7,3,0);
    //printf("a7:");
    //printBitArray(4,a7);
    //printf("\n");

    addTableRow(t,a1);
    printf("a7:");
    printBitArray(4,a7);
    printf("\n");
    //addTableRow(t,a2);
    printf("a7:");
    printBitArray(4,a7);
    printf("\n");
    //addTableRow(t,a3);
    printf("a7:");
    printBitArray(4,a7);
    printf("\n");
    //addTableRow(t,a4);
    printf("a7:");
    printBitArray(4,a7);
    printf("\n");
    //addTableRow(t,a5);
    printf("a7:");
    printBitArray(4,a7);
    printf("\n");
    addTableRow(t,a6);
    printf("after add a6, a7:");
    printBitArray(4,a7);
    printf("\n");
    addTableRow(t,a7);
    printf("a7:");
    printBitArray(4,a7);
    printf("\n");

    printTable(t);
    return t;
}*/

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

int main (void) {

    table t = sampleTable();
    printf("main\n");
    printTable(t);
    printf("\n");

    table ps = Table(0,4);
    table tt = loop(t, &ps);
    printTable(tt);
    return 0;
}





