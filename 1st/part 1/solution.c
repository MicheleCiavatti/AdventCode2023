#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define FILENAME "input.txt"

int main () {
    FILE *f;
    char buf[500];
    int total = 0;

    f = fopen(FILENAME, "r"); assert(f != NULL);

    while (fgets(buf, sizeof(buf), f)) {
        char c;
        int i = 0;
        int n = 0;
        int arr[20];
        while ((c = buf[i++]) != '\0') {
            if (isdigit(c)) {
                arr[n++] = c - '0';
            }
        }
        printf("Total before: %d", total);
        total += arr[0] * 10 + arr[n - 1];
        printf("    Total after: %d\n", total);
    }
    printf("total = %d", total);
    return 0;
}