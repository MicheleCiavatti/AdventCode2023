#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "solution.h"

#define FILENAME "input.txt"
#define DIGITS 9

digit_t digits[] = {
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

char buf[500];

int compute(int *arr, int *pos, int length, int n, digit_t digit) {
    char *r = NULL;
    char tmp[length];
    strcpy(tmp, buf);
    while ((r = strstr(tmp, digit.stringFormat)) != NULL) {
        int position = r - tmp;
        pos[n] = position;
        arr[n++] = digit.numFormat;
        tmp[position] = 'z';
    }    
    return n;
}

int getMin(int *arr, int *pos, const int n) {
    int min = 0;
    for (int i = 1; i < n; i++) {
        if (pos[min] > pos[i]) {
            min = i;
        }
    }
    return arr[min];
}

int getMax(int *arr, int *pos, const int n) {
    int max = 0;
    for (int i = 0; i < n; i++) {
        if (pos[max] < pos[i]) max = i;
    }
    return arr[max];
}

void printArrays(const int* arr, const int *pos, const int n) {
    printf("From: [");
    for (int i = 0; i < n; i++) {
        printf("(%d, %d)%s", arr[i], pos[i], (i == n-1) ? "]\n" : ", ");
    }
}

int main () {
    FILE *f;
    int total = 0;

    f = fopen(FILENAME, "r"); assert(f != NULL);

    while (fgets(buf, sizeof(buf), f)) {
        char c;
        int *arr;
        int *pos;
        arr = (int*)malloc(20 * sizeof(int));
        pos = (int*)malloc(20 * sizeof(int));
        int length = 0;
        int n = 0;
        while ((c = buf[length++]) != '\0') {
            if (isdigit(c)) {
                pos[n] = length - 1;
                arr[n++] = c - '0';
            }
        }
        for (int j = 0; j < DIGITS; j++) {
            n = compute(arr, pos, length, n, digits[j]);
        }
        printArrays(arr, pos, n);
        printf("min elem %d ; max elem = %d\n", getMin(arr, pos, n) * 10, getMax(arr, pos, n));
        total += getMin(arr, pos, n) * 10 + getMax(arr, pos, n);
        free(arr); free(pos);
    }
    printf("Total: %d", total);
}