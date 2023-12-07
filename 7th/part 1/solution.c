#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "solution.h"
#include <stdbool.h>
#include <assert.h>

#define INPUT "input.txt"
#define CARDS_PER_HAND 5

char buf[200];
set_t set_input;
set_t set_sorted;

void printSorted() {
    printf("SORTED: We have %d hands.\n", set_sorted.n);
    for (int i = 0; i < set_sorted.n; i++) {
        printf("HAND: ");
        for (int j = 0; j < CARDS_PER_HAND; j++) {
            printf("%d ", set_sorted.hands[i].cards[j]);
        }
        printf("- VALUE: %lu - POWER: %d - BID: %d\n", set_sorted.hands[i].val, set_sorted.hands[i].power, set_sorted.bids[i]);
    }
}

void printInput() {
    printf("INPUT: We have %d hands.\n", set_input.n);
        for (int i = 0; i < set_input.n; i++) {
        printf("HAND: ");
        for (int j = 0; j < CARDS_PER_HAND; j++) {
            printf("%d ", set_input.hands[i].cards[j]);
        }
        printf("- VALUE: %lu - POWER: %d - BID: %d\n", set_input.hands[i].val, set_input.hands[i].power, set_input.bids[i]);
    }
}

unsigned int myPow(const unsigned int base, const unsigned int exp) {
    unsigned int total = 1L;
    for (int i = 0; i < exp; i++) {
        total *= base;
    }
    return total;
}

void swapArr(int arr1[], int arr2[], const int length) {
    for (int i = 0; i < length; i++) {
        int tmp = arr1[i];
        arr1[i] = arr2[i];
        arr2[i] = tmp;
    }
}

void swapValues(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swapUInts(unsigned int *a, unsigned int *b) {
    unsigned int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap(int max_idx, int index) {
    swapArr(set_sorted.hands[max_idx].cards, set_sorted.hands[index].cards, CARDS_PER_HAND);
    swapValues(&set_sorted.bids[index], &set_sorted.bids[max_idx]);
    swapUInts(&set_sorted.hands[index].val, &set_sorted.hands[max_idx].val);
}

void selectionSort(const int start, const int stop)
{
    int i, j, max_idx;
    for (i = start; i < stop - 1; i++) {
        max_idx = i;
        for (j = i + 1; j < stop; j++) {
            if (set_sorted.hands[j].val > set_sorted.hands[max_idx].val) max_idx = j;
        }
        if (max_idx != i) {
            swap(max_idx, i);
        }
            
    }
    set_sorted.n = stop;
}

bool isPresent(const int val) {
    for (int i = 0; i < set_sorted.n; i++) {
        if (val == set_sorted.hands[i].val) return true;
    }
    return false;
}

void addHand(const int inputIndex, const int power) {
    if (isPresent(set_input.hands[inputIndex].val)) {
        return;
    }
    
    for (int j = 0; j < CARDS_PER_HAND; j++) {
        set_sorted.hands[set_sorted.n].cards[j] = set_input.hands[inputIndex].cards[j];
    }
    set_sorted.hands[set_sorted.n].power = power;
    set_sorted.bids[set_sorted.n] = set_input.bids[inputIndex];
    set_sorted.hands[set_sorted.n].val = set_input.hands[inputIndex].val;
    set_sorted.n++;
}

void printArr(int arr[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
}

/*
A function that searches if a given arr has exactly n_valEquals values that are the same.
If true, the integer that repeats in the array for n_valEquals is saved in third parameter.
*/
bool searchEqualValues(int arr[], int length, int *p_match, int n_valEquals) {
    for (int i = 0; i < length; i++) {
        int counter = 0;
        for (int j = 0; j < length; j++) {
            if (arr[i] == arr[j]) counter++;
        }
        if (counter == n_valEquals) {
            *p_match = arr[i];
            return true;
        }
    }
    return false;
}

bool searchDifferentPair(const int arr[], const int length, int *p_match, const int exclude) {
    for (int i = 0; i < length; i++) {
        int counter = 0;
        if (arr[i] == exclude) continue; 
        for (int j = 0; j < length; j++) {
            if (arr[i] == arr[j]) counter++;
        }
        if (counter == 2) {
            *p_match = arr[i];
            return true;
        }
    }
    return false;
}

void fiveOfAKind() {
    int start = set_sorted.n;
    for (int i = 0; i < set_input.n; i++) {
        int match;
        if (searchEqualValues(set_input.hands[i].cards, CARDS_PER_HAND, &match, CARDS_PER_HAND))
            addHand(i, 6);
    }
    selectionSort(start, set_sorted.n);
}

void fourOfAKind() {
    int start = set_sorted.n;
    for (int i = 0; i < set_input.n; i++) {
        int match;
        if (searchEqualValues(set_input.hands[i].cards, CARDS_PER_HAND, &match, 4))
            addHand(i, 5);
    }
    selectionSort(start, set_sorted.n);
}

void fullHouse() {
    int start = set_sorted.n;
    for (int i = 0; i < set_input.n; i++) {
        int pair;
        int tris;
        if (    searchEqualValues(set_input.hands[i].cards, CARDS_PER_HAND, &pair, 2)
            &&  searchEqualValues(set_input.hands[i].cards, CARDS_PER_HAND, &tris, 3)) {
            addHand(i, 4);
        }
    }
    selectionSort(start, set_sorted.n);
}

void threeOfAKind() {
    int start = set_sorted.n;
    for (int i = 0; i < set_input.n; i++) {
        int tris;
        if (searchEqualValues(set_input.hands[i].cards, CARDS_PER_HAND, &tris, 3)) {
            addHand(i, 3);
        }
    }
    selectionSort(start, set_sorted.n);
}

void doublePair() {
    int start = set_sorted.n;
    for (int i = 0; i < set_input.n; i++) {
        int pair1 = -1, pair2 = -1;
        if (    searchEqualValues(set_input.hands[i].cards, CARDS_PER_HAND, &pair1, 2) 
            &&  searchDifferentPair(set_input.hands[i].cards, CARDS_PER_HAND, &pair2, pair1)) {
            addHand(i, 2);
        }
    }
    selectionSort(start, set_sorted.n);
}

void onePair() {
    int start = set_sorted.n;
    for (int i = 0; i < set_input.n; i++) {
        int pair;
        if (searchEqualValues(set_input.hands[i].cards, CARDS_PER_HAND, &pair, 2)) {
            addHand(i, 1);
        }
    }
    selectionSort(start, set_sorted.n);
}

void highCard() {
    int start = set_sorted.n;
    for (int i = 0; i < set_input.n; i++) {
        addHand(i, 0);
    }
    selectionSort(start, set_sorted.n);
}

void sortHands() {
    set_sorted.n = 0;
    fiveOfAKind();
    fourOfAKind();
    fullHouse();
    threeOfAKind();
    doublePair();
    onePair();
    highCard();
}



int main() {
    FILE *f;
    f = fopen(INPUT, "r");
    unsigned int total = 0;
    while (fgets(buf, sizeof(buf), f)) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
        char *p;
        set_input.hands[set_input.n].val = 0L;
        p = strtok(buf," "); //HAND
        for (int i = 0; i < CARDS_PER_HAND; i++) {
            if (isdigit(p[i])) set_input.hands[set_input.n].cards[i] = p[i] - '0';
            else {
                switch (p[i]) {
                    case 'A': set_input.hands[set_input.n].cards[i] = 14; break;
                    case 'K': set_input.hands[set_input.n].cards[i] = 13; break;
                    case 'Q': set_input.hands[set_input.n].cards[i] = 12; break;
                    case 'J': set_input.hands[set_input.n].cards[i] = 11; break;
                    default: set_input.hands[set_input.n].cards[i] = 10; 
                }
            }
        /*-------------------------------------EXPLANATION---------------------------------------------*/
        /*
        At each hand we assign a single integer value, to semplify the next sorting. To meet the 
        requirements of the exercise, we consider that the max value possibile is 'A' which turns into
        14 with our mapping, and then map the hand as a 14-based number.
        */
            set_input.hands[set_input.n].val += set_input.hands[set_input.n].cards[i] * myPow(14L, CARDS_PER_HAND - i - 1);
        }
        set_input.hands[set_input.n].power = -1;
        p = strtok(NULL, " "); //BID
        set_input.bids[set_input.n++] = atoi(p);
    }
    printInput();
    sortHands();
    printSorted();

    for (int i = 0; i < set_sorted.n; i++) {
        printf("Adding to total %d * %d\n", set_sorted.bids[i], set_sorted.n - i);
        total += set_sorted.bids[i] * (set_sorted.n - i);
    }
    printf("Total: %lu\n", total);
}
