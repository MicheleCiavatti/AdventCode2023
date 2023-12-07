#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "solution.h"
#include <stdbool.h>
#include <assert.h>

#define INPUT "input.txt"

input_t table;
char buf[100000];

void allocate(const bool isTime) {
    char l[10000];
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
    int length = 0;
    while (p != NULL) {
        if (isTime) table.t[length++] = atoi(p); 
        else table.d[length++] = atoi(p);
        p = strtok(NULL, " ");
    }
    table.n = length;
}

void printTable() {
    printf("Time - Distance\n");
    for (int i = 0; i < table.n; i++) {
        printf("time=%d - distance=%d\n", table.t[i], table.d[i]);
    }
}

int compute(const int index) {
    int maxTime = table.t[index];
    int record = table.d[index];
    int count = 0;
    for (int charge = 0; charge < maxTime; charge++) {
        if (charge * (maxTime - charge) > record)
            count++; 
    }
    return count == 0 ? 1 : count;
}

int main() {
    FILE *f;
    int total = 1;
    f = fopen(INPUT, "r"); assert(f != NULL);
    fgets(buf, sizeof(buf), f);
    buf[strlen(buf)] = '\0';
    allocate(true);
    fgets(buf, sizeof(buf), f);
    buf[strlen(buf) + 1] = '\0';
    allocate(false);
    fclose(f);
    printTable();
    for (int i = 0; i < table.n; i++) {
        total *= compute(i);
    }
    printf("Total = %d", total);
}