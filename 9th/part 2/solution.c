#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#define INPUT "input.txt"
#define MAX_NUMBERS_PER_LINE 100
#define MAX_ITERATIONS 1000

char buf[500];


void printLongArr(const long arr[], const int length) {
    for (int i = 0; i < length; i++) {
        printf("%ld ", arr[i]);
    }
    printf("\n");
}

bool isFinished(long nums[], int length) {
    for (int i = 0; i < length; i++) {
        if (nums[i] != 0) return false;
    }
    return true;
}

long myReduction(const long a[], const int n) {
    long out = 0;
    for (int i = n - 1; i >= 0; i--) {
        out = a[i] - out;
    }
    return out;
}

long computeLine(long nums[], int length) {
    long first_nums[MAX_ITERATIONS]; int n = 0;
    while (!isFinished(nums, length)) {
        first_nums[n++] = nums[0];
        for (int i = 0; i < length - 1; i++) {
            nums[i] = nums[i+1] - nums[i];
        }
        length--;
    }
    printLongArr(first_nums, n);
    return myReduction(first_nums, n);
}

int main() {
    FILE *f;
    f = fopen(INPUT, "r"); assert(f != NULL);
    long total = 0;
    while (fgets(buf, sizeof(buf), f)) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0'; 
        long nums[MAX_NUMBERS_PER_LINE];
        int length = 0;
        char *p;
        p = strtok(buf, " ");
        while (p != NULL) {
            char *ptr;
            nums[length++] = strtol(p, &ptr, 10); 
            p = strtok(NULL, " ");
        }
        printLongArr(nums, length);
        total += computeLine(nums, length);
    }
    printf("Total: %ld\n", total);
}