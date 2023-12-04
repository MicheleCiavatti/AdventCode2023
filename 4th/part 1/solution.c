#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define INPUT "input.txt"
#define DELIMITER_GAME ':'
#define DELIMITER_NUMS "|"

char buf[1000];
char card[1000];
char *winnings; int winningInt[30];
char *nums;     int numsInt[50];

/*A simple function to calculate base^exp for integers. It works only for base >= 0.*/
int myPow(int base, int exp) {
    int out = 1;
    for (int i = 0; i < exp; i++) {
        out *= base;
    }
    return out;
}

/*Remove the beginning of a line that contains Game ID: some_spaces*/
void trimGame(char delimiter) {
    char c;
    int i = 0;
    while ((c = buf[i++]) != delimiter);
    while (isspace(c = buf[i++]));
    int j = 0;
    do {
        card[j++] = c;
        c = buf[i++];
    } while (c != '\0');
    card[j] = '\0';
}

void divideNumbers(char *delimiter) {
    winnings = strtok(card, delimiter);
    nums = strtok(NULL, delimiter);
}

/*Returns the amount of winning numbers*/
int fillWinningNumbers() {
    char *p;
    int length = 0;
    p = strtok(winnings, " ");
    while (p != NULL) {
        winningInt[length++] = atoi(p);
        p = strtok(NULL, " ");
    }
    return length;
}

/*Returns the amount of attempt numbers*/
int fillNumbers() {
    char *p;
    int length = 0;
    p = strtok(nums, " ");
    while (p != NULL) {
        numsInt[length++] = atoi(p);
        p = strtok(NULL, " ");
    }
    return length;
}

int comuputeCard() {
    trimGame(DELIMITER_GAME);
    divideNumbers(DELIMITER_NUMS);
    int lengthWinnings = fillWinningNumbers();
    int lengthAttempts = fillNumbers();
    int count = -1;
    for (int i = 0; i < lengthAttempts; i++) {
        int val = numsInt[i];
        for (int j = 0; j < lengthWinnings; j++) {
            if (val == winningInt[j]) {
                count++;
                break;
            }
        }
    }
    return count == -1 ? 0 : myPow(2, count);
}

int main () {
    FILE *f;
    f = fopen(INPUT, "r"); assert(f != NULL);
    int total = 0;
    while (fgets(buf, sizeof(buf), f)) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0'; //Eliminates '\n' at the end.
        total += comuputeCard();
    }
    printf("Total: %d", total);
    return 0;
}