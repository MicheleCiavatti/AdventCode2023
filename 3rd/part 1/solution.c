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

/*---------------------------------------EXPLANATION---------------------------------------*/
/*Given [i,j] indexes of a symbol not digit and not '.', we need to check its sorroundings
for digits
                                [i-1,j-1]    [i-1,j  ]    [i-1,j+1]
                                [i  ,j-1]    [i  ,j  ]    [i  ,j+1]
                                [i+1,j-1]    [i+1,j  ]    [i+1,j+1]
Let's assume that we found a digit in position [n,m]. To compute the number, we need to:
- walk backwards n, n-1, n-2, ..., until we n-k is not a digit or n-k == 0. Every time we find a digit in
position [n][n-k], we modify mat[n][n-k] = '.'. We create an array
    BackArr = [n, n-1, ..., n-k] ----REVERSED---> [n-k, ..., n-1,n] 
- walk onwards n, n+1, n+2, ..., untile n+h is not a digit or n+h == COLUMNS. Every time we find a digit in
position [n][n-k], we modify mat[n][n-k] = '.'. We create an array
    OnArr = [n, n+1, ...]
Then we remove the last element of BackArr (n), so that we don't count it twice.
We concatenate the two arrays (they're string arrays), and use atoi on the result.
*/

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
    return strlen(backArr) > 0 ? atoi(backArr) : 0;
}

int computeAdj(const int row, const int column) {
    int i, j;
    int out = 0;

    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            if (row + i < ROWS && row + i >= 0 && column + j >= 0 && column + j < COLUMNS)
                out += computeNum(row + i, column + j);
        }
    }
    return out;
}

int main() {
    int i, j;
    int total = 0;
    readInput();
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLUMNS; j++) {
            if (mat[i][j] != '.' && !isdigit(mat[i][j])) {
                total += computeAdj(i, j);
            }
        }
    }
    printMat();
    printf("Total: %d", total);
}