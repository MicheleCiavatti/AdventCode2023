#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "solution.h"
#include <limits.h>

#define INPUT "input.txt"
#define MAX_RANGES 100
#define STEP_AMOUNT 8
#define SEED_INDEX 0
#define SOIL_INDEX 1
#define LIMIT ULLONG_MAX

char buf[10000];
int phase = 0;
range_t steps[STEP_AMOUNT];
unsigned long long min = LIMIT; 
unsigned long long max = 0;

void allocateSeeds() {
    char seedList[10000];
    char c;
    int i = 0;
    while ((c = buf[i++]) != ':');
    while (isspace(c = buf[i++]));
    int j = 0;
    do {
        seedList[j++] = c;
        c = buf[i++];
    } while (c != '\0');
    seedList[j] = '\0';
    
    unsigned long long start[30];
    unsigned long long range[30];
    i = 0;
    char *p;
    p = strtok(seedList, " ");
    while (p != NULL) {
        char *ptr;
        if (i % 2) {
            range[i/2] = strtoul(p, &ptr, 10);
            i++;
        }
        else {
            start[i/2] = strtoul(p, &ptr, 10);
            i++;
        }
        p = strtok(NULL, " ");
    }
    for (j = 0; j < i / 2; j++) {
        steps[SEED_INDEX].dst[j] = start[j];
        steps[SEED_INDEX].range[j] = range[j];
    }
    steps[SEED_INDEX].length = j;
}

void allocate() {
    if (!phase) {
        allocateSeeds();
    } else {
        char *p, *ptr;
        assert(buf != NULL);
        p = strtok(buf, " "); unsigned long long startDest = strtoul(p, &ptr, 10);
        p = strtok(NULL, " "); unsigned long long startSrc = strtoul(p, &ptr, 10);
        p = strtok(NULL, " "); unsigned long long range = strtoul(p, &ptr, 10);
        steps[phase].dst[steps[phase].length] = startDest;
        steps[phase].src[steps[phase].length] = startSrc;
        steps[phase].range[steps[phase].length] = range;
        steps[phase].length++;
    }
}

bool isInRange(unsigned long long num, unsigned long long startRange, unsigned long long endRange) {
    return num >= startRange && num < endRange;
}

bool isMatching(unsigned long long start, unsigned long long src, unsigned long long r, unsigned long long range) {
    return (    (start >= src && start < src + range)
            ||  (start + r >= src && start + r < src + range));
}

/*---------------------------------------------------EXPLANATION------------------------------------------------------*/
/*
A layer of the mapping is defined with variable type range_t, which has arrays src[], dst[], range[] and length of 
these arrays.
We call a layer as a "step", and we read all the steps from the input file and memorize them in variable steps[].
For each single step, we add a padding: intervals not explicited are added to the step, whose mapping is an identity
function.
For example, suppose that step[1] doesn't cover interval for sources 0 - 20: therefore, we add another interval in
step[1] at the the position step[1].length, like this: 
    step[1].src[step[1].length] = 0; step[1].dst[step[1].length] = 0; step[1].range[step[1].length++] = 20
Let's consider a specific interval I:
                                    I
            |start|----------------------------------|start+range|
We pass I in every step: since we added padding, every single sub-interval of I will have a match at some point.
Let's also consider a specific interval of src[] (r is a range) called S.
                                   S
            |src|------------------------------------|src+r|
If I and S do not intersecate, I simply goes to the next interval of src[].
If I and S do intersecate (we find out with function isMatching()), then we calculate the subinterval that matches.
Ex:
            |start|-------------------------------------|start+range|
                                |src|------------------------------------------|src+r|
                                    |___________________|
                                      MATCHING INTERVAL
We apply the mapping given by the step to the matching interval, and the matching interval goes to the next step.
The interval I continues to the next src[], until finished.
At step 8, the last, an interval J arrives to the compute() function (the recursive function we've been using) and
it only checks if it's Jstart is < than min: if so, then min = Jstart.
*/

void compute(unsigned long long start, unsigned long long range, const int index) {
    printf("START: %llu - RANGE: %llu\n", start, range);
    if (index < STEP_AMOUNT) {
        for (int i = 0; i < steps[index].length; i++) {
            printf("COMPARING WITH SRC: %llu - RANGE: %llu\n", steps[index].src[i], steps[index].range[i]);
            if (isMatching(start, steps[index].src[i], range, steps[index].range[i])) {
                printf("A match with above!\n");
                unsigned long long newStart = start > steps[index].src[i] ? start : steps[index].src[i];
                unsigned long long end = start + range < steps[index].src[i] + steps[index].range[i] 
                    ? start + range 
                    : steps[index].src[i] + steps[index].range[i];
                unsigned long long newRange = end - newStart;
                printf("NEWSTART: %llu -> %llu - NEWRANGE: %llu\n", newStart, steps[index].dst[i] + newStart - steps[index].src[i], newRange);
                compute(steps[index].dst[i] + newStart - steps[index].src[i], newRange, index + 1);
            }
        }
    } else {
        printf("------------------------Comparing MIN=%llu and START=%llu\n", min, start);
        if (start < min) {
            min = start;
        }
        if (start + range > max) max = start + range;
    }
}

void swap(unsigned long long* xp, unsigned long long* yp) {
    unsigned long long temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
}

void sortStep(const int index) {
    int i, j, min_idx;
    for (i = 0; i < steps[index].length - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < steps[index].length; j++) {
            if (steps[index].src[j] < steps[index].src[min_idx]) {
                min_idx = j;
            }
        }
        /*------------3 exchanges must be done------------*/
        swap(&steps[index].src[i], &steps[index].src[min_idx]);
        swap(&steps[index].dst[i], &steps[index].dst[min_idx]);
        swap(&steps[index].range[i], &steps[index].range[min_idx]);
    }
}

void padding(const int index) {
    int finalLength = steps[index].length;
    for (int i = 0; i < steps[index].length - 1; i++) {
        unsigned long long end1 = steps[index].src[i] + steps[index].range[i]; 
        unsigned long long start2 = steps[index].src[i+1];
        if (end1 < start2) {
            steps[index].src[finalLength] = end1;
            steps[index].dst[finalLength] = end1;
            steps[index].range[finalLength++] = start2 - end1;
        }
    }
    if (steps[index].src[0] != 0) {
        steps[index].src[finalLength] = 0;
        steps[index].dst[finalLength] = 0;
        steps[index].range[finalLength++] = steps[index].src[0] - 1;
    }
    if (steps[index].src[steps[index].length - 1] + steps[index].range[steps[index].length - 1] < LIMIT) {
        steps[index].src[finalLength] = steps[index].src[steps[index].length - 1] + steps[index].range[steps[index].length - 1];
        steps[index].dst[finalLength] = steps[index].src[steps[index].length - 1] + steps[index].range[steps[index].length - 1];
        steps[index].range[finalLength++] = LIMIT - steps[index].src[steps[index].length - 1];
    }
    steps[index].length = finalLength;
}

void printStep(const int index) {
    printf("Printing step %d\n", index);
    for (int i = 0; i < steps[index].length; i++) {
        printf("DST=%llu | SRC=%llu | RANGE=%llu\n", steps[index].dst[i], steps[index].src[i], steps[index].range[i]);
    }
}

int main () {
    FILE *f;
    f = fopen(INPUT, "r"); assert(f != NULL);
    for (int i = 0; i < STEP_AMOUNT; i++) {
        steps[i].length = 0;
    }
    while (fgets(buf, sizeof(buf), f)) {
        if (strstr(buf, "map")) continue;
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        if (strlen(buf) == 0) { phase++; continue; }
        allocate();
    }
    fclose(f);
    for (int i = 1; i < STEP_AMOUNT; i++) {
        sortStep(i);
        padding(i);
    }
    for (int i = 1; i < STEP_AMOUNT; i++) {
        printStep(i);
    }
    for (int i = 0; i < steps[SEED_INDEX].length; i++) {
        compute(steps[SEED_INDEX].dst[i], steps[SEED_INDEX].range[i], SOIL_INDEX);
    }
    printf("Min = %llu; Max = %llu; Max ULLONG = %llu", min, max, LIMIT);
}