#ifndef __SOLUTION_H__
#define __SOLUTION_H__

typedef struct node {
    unsigned int left;
    unsigned int right;
} node_t;

typedef struct arr {
    char array[300];
    unsigned int length;
    unsigned int cursor;
} arr_t;

#endif