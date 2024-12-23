#ifndef BIT_H
#define BIT_H

#include <stdint.h>
#define MAX_BIT 64 * 4
#define INT_ARRAY_SIZE (MAX_BIT / 64)
#define INT_INDEX(i) (i / 32)
#define BIT_INDEX(i) ((i % 32) * 2)

typedef enum bit {
    F = 0,
    T = 1,
    X = 3
} bit;

struct bitArray {
   uint64_t array[INT_ARRAY_SIZE];
};

typedef struct bitArray *bitArray;

bitArray BitArray();
void setBit(bitArray bs, int i, bit v);
bit getBit(bitArray bs, int i);
bitArray copyBitArray(bitArray);
void printBitArray(int n, bitArray bs);
int bitArrayEq(bitArray a, bitArray b);


#endif

