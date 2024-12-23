#include <gc/gc.h>
#include <gc.h>
#include <stdio.h>
#include "bit.h"

bitArray BitArray () {
    bitArray bs = GC_MALLOC(sizeof(struct bitArray));
    for (int i = 0; i < INT_ARRAY_SIZE; i++) {
        bs->array[i] = 0;
    }
    return bs;
}

void setBit(bitArray bs, int i, bit v) {
    uint64_t v64 = (uint64_t)v;
    bs->array[INT_INDEX(i)] &= ~(0x11 << BIT_INDEX(i));
    bs->array[INT_INDEX(i)] |= (v64 << BIT_INDEX(i));
}

bit getBit(bitArray bs, int i) {
    int bi = BIT_INDEX(i);
    int ii = bs->array[INT_INDEX(i)];
    int ui = ii & (1 << bi);
    int li = ii & (1 << (bi+1));
    return (ui | li) >> bi;
}

bitArray copyBitArray(bitArray bs) {
    bitArray newbs = BitArray();
    for (int i = 0; i < INT_ARRAY_SIZE; i++) {
        newbs->array[i] = bs->array[i];
    }
    return newbs;
}

void printBitArray(int n, bitArray bs) {
    for (int i = 0; i < n; i++) {
        bit b = getBit(bs, i);
        if (b == T) {
            printf("1,");
        } else if (b == F) {
            printf("0,");
        } else {
            printf("X,");
        }
    }
    //printf("\n");
}

int bitArrayEq(bitArray a, bitArray b) {
    for (int i = 0; i < INT_ARRAY_SIZE; i++) {
        //printf("beq:%d,%d\n", a->array[i], b->array[i]);
        if (a->array[i] == b->array[i]) {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
}

/*
int main(void) {
    bitArray bs = BitArray();
    setBit(bs,0,F); 
    setBit(bs,1,T);
    setBit(bs,2,T);
    setBit(bs,3,X);
    printf("%d\n", getBit(bs,0));
    printf("%d\n", getBit(bs,1));
    printf("%d\n", getBit(bs,2));
    printf("%d\n", getBit(bs,3));
}
*/
