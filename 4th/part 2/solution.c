#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define INPUT "input.txt"
#define DELIMITER_GAME ':'
#define DELIMITER_NUMS "|"
#define NUM_CARDS 211

char buf[1000];
char card[1000];
char *winnings; int winningInt[30];
char *nums;     int numsInt[50];
int cards[NUM_CARDS];

/*----------------------------------------------EXPLANATION----------------------------------------------*/
/*
We initialize an array cards[211] with ones:
    [1, 1, 1, ..., 1]
For each card, we compute the numbers of cards won and update the array cards[].
For example, the first card (card[0]) has 2 cards won. 
Therefore, we increment cards[1] and cards[2] a number of times equal to card[0] (1).
In general, given a card[i] with j wins, we compute:
- cards[i + 1] += card[i]
- cards[i + 2] += card[i]
- ...
- cards[i + j] += card[i]
In this computation, we need to check to never exceed index 210 of array cards[].
In the end, we operate a reduction sum for the array cards[] to get the total amount of cards we won.
*/

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

void printCards() {
    printf("cards[] = [ ");
    for (int i = 0; i < NUM_CARDS; i++) {
        printf("%d ", cards[i]);
    }
    printf("]\n");
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

void modifyCards(const int i, const int j) {
    printf("Modifying cards since cards[%d] has won %d times\n", i, j);
    for (int k = 1; k <= j; k++) {
        int index = i + k;
        printf("%d\t", index);
        if (index < NUM_CARDS) 
            cards[index] = cards[index] + cards[i];
    }
}

/*The ID is not the actual Game ID but rather Game ID-1*/
void comuputeCard(const int ID) {
    printf("\nID = %d\n", ID);
    trimGame(DELIMITER_GAME);
    divideNumbers(DELIMITER_NUMS);
    int lengthWinnings = fillWinningNumbers();
    int lengthAttempts = fillNumbers();
    int countWon = 0; //Number of cards won at game ID
    for (int i = 0; i < lengthAttempts; i++) {
        int val = numsInt[i];
        for (int j = 0; j < lengthWinnings; j++) {
            if (val == winningInt[j]) {
                countWon++;
                break;
            }
        }
    }
    printCards();
    if (countWon > 0) modifyCards(ID, countWon);
    printf("\n------------FINISH ITERATION--------------\n");
}

int main () {
    FILE *f;
    f = fopen(INPUT, "r"); assert(f != NULL);
    for (int i = 0; i < NUM_CARDS; i++) {
        cards[i] = 1;
    }
    int i = 0;
    unsigned long total = 0;
    while (fgets(buf, sizeof(buf), f)) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0'; //Eliminates '\n' at the end.
        comuputeCard(i++);
    }
    for (int j = 0; j < NUM_CARDS; j++) {
        total += cards[j];
    }
    printf("Total: %lu", total);
    return 0;
}