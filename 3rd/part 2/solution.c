#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#define POINT_INT 46
#define INPUT "input.txt"
#define COLUMNS 140
#define ROWS 140

char mat[ROWS][COLUMNS];

// function to reverse a string
void strrev(char* str)
{
    // if the string is empty
    if (!str) {
        return;
    }
    // pointer to start and end at the string
    int i = 0;
    int j = strlen(str) - 1;
 
    // reversing string
    while (i < j) {
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
        i++;
        j--;
    }
}

void readInput() {
    FILE *f;
    f = fopen(INPUT, "r"); assert(f != NULL);
    char c;
    int i = 0, j = 0;
    while ((c = fgetc(f)) != EOF) {
        if (c != '\n') {
            mat[i][j++] = c;
            if (j >= COLUMNS) {
                i++;
                j = 0;
            }
        }
    }
    fclose(f);
}

void printMat() {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLUMNS; j++) {
            printf("%c", mat[i][j]);
        }
        printf("\n");
    }
}

int computeNum(int row, int col) {
    printf("Computing in row %d and column %d\t", row, col);
    char backArr[COLUMNS+1]; int backLength = 0;
    char onArr[COLUMNS+1]; int onLength = 0;
    int j = 0;
    char ch;
    /*-----------BACKWARDS------------*/
    ch = mat[row][col];
    while (col - j >= 0 && isdigit(ch)) {
        if (col - j != col) 
            mat[row][col-j] = '.';
        backArr[j++] = ch;
        ch = mat[row][col - j];
    }
    backLength = j;
    backArr[backLength] = '\0';
    strrev(backArr);
    if (isdigit(backArr[backLength - 1])) backArr[backLength - 1] = '\0'; 
    printf("backArr: %s\t\t", backArr);
    /*--------------------------------*/
    j = 0;
    ch = mat[row][col];
    /*------------ONWARDS-------------*/
    while (col + j < COLUMNS && isdigit(ch)) {
        mat[row][col+j] = '.';
        onArr[j++] = ch;
        ch = mat[row][col + j];
    }
    onLength = j;
    onArr[onLength] = '\0';
    printf("onArr: %s\n", onArr);
    /*--------------------------------*/
    /*----------CONCATENATE-----------*/
    strcat(backArr, onArr);
    if (strlen(backArr) > 0) printf("concatenated: %s\n", backArr);
    return strlen(backArr) > 0 ? atoi(backArr) : -1;
}

int computeAdj(const int row, const int column) {
    int i, j;
    int nums[10] = {-1};
    int k = 0;
    printf("\nFound * at position [%d][%d]\n", row, column);
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            if (row + i < ROWS && row + i >= 0 && column + j >= 0 && column + j < COLUMNS) {
                int val = computeNum(row + i, column + j);
                if (val != -1) nums[k++] = val;
            }
        }
    }
    printf("nums = ");
    for (int m = 0; m < k; m++) {
        printf("%d ", nums[m]);
    }
    return k > 2 ? 0 : nums[0] * nums[1];
}

int main() {
    int i, j;
    int total = 0;
    readInput();
    printf("%d", '*');
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLUMNS; j++) {
            if (mat[i][j] == 42) {
                total += computeAdj(i, j);
            }
        }
    }
    printf("Total: %d", total);
}