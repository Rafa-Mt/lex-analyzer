#include "stringfuncs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* slice(char* entry, int start, int end) {
    int strsize = strlen(entry);
    if (end > strsize || start < 0 || !*entry) {
        perror("Invalid values");
        return NULL;
    }
    int finalsize = end-start+1;
    char* out = (char*) malloc(finalsize*sizeof(char));

    for (int i=start, actual=0; i<=end; i++, actual++) {
        if (i == end) {
            out[actual] = '\0';
            break;
        }
        out[actual] = entry[i];
    }

    return out;
}

char* trim(char* entry) {
    if (!*entry) {
        perror("Invalid values");
        return NULL;
    }

    int lenght = strlen(entry);
    int left = 0;
    int right = lenght-1;
    for (int i=0; i<lenght; i++) {
        if (!isspace(entry[i])) break;
        left++;
    }
    for (int i=lenght-1; i>0; i--) {
        if (!isspace(entry[i])) break;
        right--;
    }

    return slice(entry, left, right+1);
}

int isNum(char entry) {
    return entry >= '0' && entry <= '9';
}

int isLetter(char entry) {
    return entry >= 'A' && entry <= 'Z' || entry >= 'a' && entry <= 'z';
}