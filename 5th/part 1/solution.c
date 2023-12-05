#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define INPUT "input.txt"
#define MAX_SEEDS 50
/*----------------------------------------------EXPLANATION----------------------------------------------*/
/*
Each map represents: DESTINATION SOURCE RANGE_LENGTH
The first phase is about the SEED: we need to extract the number of every seed.
Then, we proceed: SEED -> SOIL -> FERTILIZER -> WATER -> LIGHT -> TEMPERATURE -> HUMIDITY -> LOCATION
*/

char buf[10000];
int phase = 0;
unsigned long seeds[MAX_SEEDS];
unsigned long temp[MAX_SEEDS];
bool indexes[MAX_SEEDS] = { false };
int seedCount = 0;
unsigned long count;

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
    
    char *p;
    p = strtok(seedList, " ");
    while (p != NULL) {
        char *ptr;
        seeds[seedCount++] = strtoul(p, &ptr, 10);
        p = strtok(NULL, " ");
    }
}

void step() {
    printf("seeds[] = ");
    for (int i = 0; i < seedCount; i++) {
        if (indexes[i]) {
            seeds[i] = temp[i];
            indexes[i] = false;
        }
        printf("%lu ", seeds[i]);
    }
    printf("\n");
}

void computeRange() {
    char *p;
    char *ptr;
    assert(buf != NULL);
    ptr = strtok(buf, " ");
    unsigned long startDest = strtoul(ptr, &p, 10);
    ptr = strtok(NULL, " ");
    unsigned long startSrc = strtoul(ptr, &p, 10);
    ptr = strtok(NULL, " ");
    unsigned long range = strtoul(ptr, &p, 10);
    for (int j = 0; j < seedCount; j++) {
        if (seeds[j] >= startSrc && seeds[j] < startSrc + range) {
            temp[j] = startDest + seeds[j] - startSrc;
            indexes[j] = true;
        }
    }
}

int main () {
    FILE *f;
    f = fopen(INPUT, "r"); assert(f != NULL);
    while (fgets(buf, sizeof(buf), f)) {
        if (strstr(buf, "map")) continue;
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        if (strlen(buf) == 0) {
            step();
            count = 0;
            phase++;
        } else {
            if (!phase) {
                allocateSeeds();
            } else {
                printf("Hello\n");
                computeRange();                
            }
        }
    }
    step();
    unsigned long min = seeds[0];
    for (int i = 0; i < seedCount; i++) {
        printf("%d ", seeds[i]);
        if (seeds[i] < min) min = seeds[i];
    }
    printf("\nMin: %lu", min);
}