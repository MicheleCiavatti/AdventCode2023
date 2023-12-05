#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#define MAX_RANGES 100

typedef struct Ranges {
    unsigned long long dst[MAX_RANGES];
    unsigned long long src[MAX_RANGES];
    unsigned long long range[MAX_RANGES];
    int length;
} range_t;

#endif