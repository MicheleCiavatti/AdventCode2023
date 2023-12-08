#include <stdio.h>
#include <stdlib.h>
#include "solution.h"
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#define INPUT "input.txt"
#define ZZZ 16275
#define LEFT 'L'

char buf[500];
arr_t directions;
node_t v[ZZZ + 1];

void printDir() {
    printf("DIRECTIONS(length=%u, cursor=%u): ", directions.length, directions.cursor);
    for (int i = 0; i < directions.length; i++) {
        printf("%c", directions.array[i]);
    }
    printf("\n");
}

unsigned int myPow(const unsigned int base, const unsigned int exp) {
    int r = 1;
    for (int i = 0; i < exp; i++) {
        r *= base;
    }
    return r;
}

unsigned int readId(const char *p) {
    unsigned int id = 0;
    for (int i = 0; i < 3; i++) {
        id += (p[i] - 'A') * myPow('Z' - 'A', 2 - i);
    }
    return id;
}

unsigned int readNeighbour(char *p) {
    char c;
    char ptr[10] = {'\0'};
    int i = 0;
    int j = 0;
    c = p[i++];
    while (isspace(c) || c == '(') c = p[i++];
    while (c != '\0') {
        ptr[j++] = c;
        c = p[i++];
    }
    return readId(ptr);
}

int main() {
    FILE *f;
    f = fopen(INPUT, "r"); assert(f != NULL);
    fgets(directions.array, sizeof(directions.array), f);
    directions.array[strlen(directions.array) - 1] = '\0';
    directions.length = strlen(directions.array);
    directions.cursor = 0;
    fgets(buf, sizeof(buf), f); //Empty line after directions
    while (fgets(buf, sizeof(buf), f)) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        char *p;
        p = strtok(buf, "=");
        unsigned int id = readId(p);
        p = strtok(NULL, ",");
        unsigned int left = readNeighbour(p);
        p = strtok(NULL, ",");
        unsigned int right = readNeighbour(p);
        v[id].left = left;
        v[id].right = right;
    } 
    fclose(f);
    printDir();
    unsigned long countSteps = 0;
    unsigned int current = 0;
    while (current != ZZZ) {
        printf("%u -> ", current);
        countSteps++;
        current = directions.array[directions.cursor] == LEFT
            ? v[current].left
            : v[current].right;
        directions.cursor = (directions.cursor + 1) % directions.length;
    }
    printf("Steps = %lu\n", countSteps);
}