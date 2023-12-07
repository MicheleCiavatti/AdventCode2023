#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "solution.h"
#include <stdbool.h>
#include <assert.h>
#include <limits.h>


#define INPUT "input.txt"

input_t table;
char buf[100000];

void allocate(const bool isTime) {
    char l[10000];
    char l2[10000] = {'\0'};
    char c;
    int i = 0;
    while ((c = buf[i++]) != ':');
    while (isspace(c = buf[i++]));
    int j = 0;
    do {
        l[j++] = c;
        c = buf[i++];
    } while (c != '\0');
    l[j++] = '\0';

    char *p;
    p = strtok(l, " ");
    while (p != NULL) {
        strcat(l2, p);
        p = strtok(NULL, " ");
    }
    printf("%s\n", l2);
    if (isTime)
        table.t = strtoull(l2, &p, 10);
    else 
        table.d = strtoull(l2, &p, 10);
}

unsigned long long compute() {
    unsigned long long maxTime = table.t;
    unsigned long long record = table.d;
    for (unsigned long long charge = 0; charge < maxTime; charge++) {
        if (charge * (maxTime - charge) >= record) {
            printf("CHARGE: %llu - REST: %llu\n", charge, maxTime - charge);
            return maxTime - 2 * charge + 1;
        }
    }
    return 0;
}

int main() {
    FILE *f;
    unsigned long long total = 0;
    f = fopen(INPUT, "r"); assert(f != NULL);
    fgets(buf, sizeof(buf), f);
    buf[strlen(buf)] = '\0';
    allocate(true);
    fgets(buf, sizeof(buf), f);
    buf[strlen(buf) + 1] = '\0';
    allocate(false);
    fclose(f);
    printf("%llu - %llu\n", table.t, table.d);
    total = compute();
    printf("Total = %d", total);
}