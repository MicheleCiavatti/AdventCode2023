#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "solution.h"

#define INPUT "input.txt"
#define MAX_GREEN 13
#define MAX_RED 12
#define MAX_BLUE 14
#define MAX_ID 10
#define MAX_SET 100
#define MAX_COLOR 6

char buf[1000];
int min_red, min_green, min_blue;

int computeBlock(const int length, char *block) {
    char c;
    char num[4];
    char color[MAX_COLOR];
    int i = 0;
    //printf("Block received: %s\n", block);
    do {
        c = block[i];
        num[i] = c;
        i++;
        //printf("%c", c);
    } while (isdigit(c));
    int val = atoi(num);
    //printf("\tval: %d\n", val);
    int j = i;
    do {
        c = block[j];
        color[j - i] = c;
        j++;
        //printf("%c", c);
    } while (c != '\0');
    if (strcmp(color, "green") == 0) {
        if (val > min_green) min_green = val;
    } else if (strcmp(color, "blue") == 0) {
        if (val > min_blue) min_blue = val;
    } else {
        if (val > min_red) min_red = val;
    }
    return 0;
}

int computeSet(const int length, char *set) {
    char c;
    int i = 0;
    int j = 0;
    char *block;
    int flag = 1;
    block = (char*)malloc((length + 1) * sizeof(char)); assert(block != NULL);
    while ((c = set[j++]) != '\0') {
        if (c != ',') {
            if (!(i == 0 && isspace(c))) block[i++] = c;
        } 
        else {
            block[i] = '\0';
            //printf("Block: %s\n", block);
            if (computeBlock(i, block) == 0) 
            {
                flag = 0;
            }
            i = 0;
        }
    }
    block[i] = '\0';
    //If the last block has \n inside we remove it.
    if (isspace(block[i-1])) {
        i--;
        block[i] ='\0';
    }
    
    //printf("Block: %s\n", block);
    if(computeBlock(i, block) == 0) {
        flag = 0;
    }
    free(block);
    return flag;
} 

/*  Using buf[toSkip] gets you immediately to the first number of the list.
    Returns 0 if the set described by the line is not possibile, 1 otherwise
*/
int computeLine(const int toSkip) {
    min_blue = min_green = min_red = 1;
    char c;
    int i = toSkip;
    int j = 0;
    char *set;
    set = (char*)malloc(MAX_SET * sizeof(char)); assert(set != NULL);
    while ((c = buf[i++]) != '\0') {
        if (c != ';') {
            if (!(j == 0 && isspace(c)))
                set[j++] = c;
        }
        else {
            set[j] = '\0';
            //printf("Set: %s\n", set);
            computeSet(j, set);
            j = 0;
        }
    }
    set[j] ='\0';
    //printf("Set: %s\n", set);
    computeSet(j, set);
    free(set);
    printf("blue: %d / green: %d / red: %d\n", min_blue, min_green, min_red);
    return min_blue * min_green * min_red;
}

/*Returns the number of characters dedicated in line for the gameId*/
int getId(int *p_id) {
    char gameId[MAX_ID];
    int length = 0;
    char c;
    while ((c = buf[length]) != ':') {
        gameId[length++] = c;
    }
    gameId[length] = '\0';
    for (int i = 0; i < length; i++) {
        if (!isdigit(gameId[i])) gameId[i] ='0';
    }
    *p_id = atoi(gameId);
    return length + 1;
}

int main() {
    FILE *f;
    int total = 0;
    f = fopen(INPUT, "r"); assert(f != NULL);
    while (fgets(buf, sizeof(buf), f)) {
        int gameId;
        int toSkip = getId(&gameId);
        int result = computeLine(toSkip);
        printf("Total = %d / toSkip = %d / ID: %d / result = %d\n", total, toSkip, gameId, result);
        total += result;
    } 
    printf("Total: %d", total);
}