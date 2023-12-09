#include <stdio.h>
#include <stdlib.h>
#include "solution.h"
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#define INPUT "input.txt"
#define ZZZ 16275
#define LEFT 'L'
#define MAX 1000

char buf[500];
arr_t directions;
node_t v[ZZZ + 1];

/*---------------------------------------EXPLANATION---------------------------------------*/
/*To avoid brute forcing, we add all nodes that terminates with an A in vecA and all 
nodes that terminate with a Z in vecZ. 
vecA = [XXA, XXA, XXA, ..., XXA] where X = any letter
vecZ = [XXZ, XXZ, XXZ, ..., XXZ] where X = any letter
We preserve the mapping that we used for the previous part, therefore vecA and vecZ will
contain unsigned integers, each obtained by the 3 chars that identify a node uniquely.
Then, for each node in vecA, we follow the instructions (LLRRLL...) given as input to 
move it across the map, and stop when it reaches a point in vecZ: we save the number of 
steps needed in an array cycles[]. We do this for every element in vecA. 
Consider i the index of element in vecA that we are processing:
    counter = 0;
- counter++. vecA[i] ----FOLLOW_DIRECTIONS----> new vecA[i]; vecA[i] is present in vecZ? NO
- counter++. vecA[i] ----FOLLOW_DIRECTIONS----> new vecA[i]; vecA[i] is present in vecZ? NO
- ...
- counter++. vecA[i] ----FOLLOW_DIRECTIONS----> new vecA[i]; vecA[i] is present in vecZ? YES
    cycles[i] = counter
In the end, we have the number of steps required for every node terminating in A to reach a 
node terminating in Z in the array cycles[]: we compute the lcm of that array, and we are 
done.*/
/*-----------------------------------------------------------------------------------------*/

void printDir() {
    printf("DIRECTIONS(length=%u, cursor=%u): ", directions.length, directions.cursor);
    for (int i = 0; i < directions.length; i++) {
        printf("%c", directions.array[i]);
    }
    printf("\n");
}

void printArr(const unsigned int a[], const unsigned int n) {
    for (int i = 0; i < n; i++) {
        printf("%u ", a[i]);
    }
    printf("\n");
}

void printArrLong(const unsigned long long a[], const unsigned int n) {
    for (int i = 0; i < n; i++) {
        printf("%lu ", a[i]);
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

bool isPresent(const unsigned int elem, const unsigned int a[], const unsigned int n) {
    for (int i = 0; i < n; i++) {
        if (elem == a[i]) return true;
    }
    return false;
}

unsigned int max(const unsigned int a[], const int n) {
    unsigned int max = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > max) max = a[i];
    }
    printf("MAX: %u\n", max);
    return max;
}

//Function returns GCD of a and b
unsigned long long findGCD(unsigned long long a, unsigned long long b){
    if(b==0)
        return a;
    return findGCD(b, a%b);
}

int main() {
    FILE *f;
    f = fopen(INPUT, "r"); assert(f != NULL);
    fgets(directions.array, sizeof(directions.array), f);
    directions.array[strlen(directions.array) - 1] = '\0';
    directions.length = strlen(directions.array);
    directions.cursor = 0;
    unsigned int countA = 0, countZ = 0;
    unsigned int vecA[MAX]; //Contains the id of elements that end with A
    unsigned int vecZ[MAX]; //Contains the id of elements that end with Z
    fgets(buf, sizeof(buf), f); //Empty line after directions
    while (fgets(buf, sizeof(buf), f)) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        char *p;
        p = strtok(buf, "="); assert(p != NULL);
        unsigned int id = readId(p);
        if (p[2] == 'A') {
            vecA[countA] = id;
            countA++;
        }
        if (p[2] == 'Z') {
            vecZ[countZ] = id;
            countZ++;
        }
        p = strtok(NULL, ","); assert(p != NULL);
        unsigned int left = readNeighbour(p);
        p = strtok(NULL, ","); assert(p != NULL);
        unsigned int right = readNeighbour(p);
        v[id].left = left;
        v[id].right = right;
    } 
    fclose(f);
    printDir();
    printf("vecA= "); printArr(vecA, countA);
    printf("vecZ= "); printArr(vecZ, countZ);

    unsigned long long cycles[countA];
    for (int i = 0; i < countA; i++) {
        directions.cursor = 0;
        unsigned long long count = -1;
        bool finished = false;
        do {
            count++;
            if (isPresent(vecA[i], vecZ, countZ)) {
                finished = true;
                cycles[i] = count;
            } else {
                vecA[i] = directions.array[directions.cursor] == LEFT
                    ? v[vecA[i]].left
                    : v[vecA[i]].right;
                directions.cursor = (directions.cursor + 1) % directions.length;
            }
        } while (!finished);
    }
    printf("Cycles: "); printArrLong(cycles, countA);
    //unsigned long countSteps = lcm(cycles, countA);
    //printf("Steps = %lu\n", countSteps);

    unsigned long long lcm = cycles[0];
    unsigned long long gcd = cycles[0];
    for (int i=1; i<countA; i++) {
        gcd = findGCD(cycles[i], lcm);
        lcm = (lcm*cycles[i])/gcd;
    }
    printf("LCM: %llu\n", lcm);
    for (int i = 0; i < countA; i++) {
        printf("%llu ", cycles[i] % lcm);
    }

}

