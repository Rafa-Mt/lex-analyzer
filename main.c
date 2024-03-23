#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stringfuncs.h"

typedef struct recordStruct {
    char* name;
    int val1;
    char* operator;
    int val2;
    int filled;
} Record;

Record* newRecord() {
    Record* result;
    result->name = malloc(65*sizeof(char));
    result->operator = malloc(3*sizeof(char));
    result->val1 = 0;
    result->val2 = 0;
    result->filled = 0;

    return result;
}

char* recordToStr(Record* record) {
    char* result = malloc(256*sizeof(char));
    sprintf(
        result,
        "{\n\tName: \"%s\"\n\tValue 1: %i\n\tOperator: \"%s\"\n\tValue 2: %i\n}", 
        record->name, record->val1, 
        record->operator, record->val2 
    );

    return result;
}


void printRecord(Record* record) {
    if (record->filled = 0) return;
    printf("Result: %s\n", recordToStr(record));
}

// Valid Syntax: "<ABRIR> [name]=[val1][operator][val2] <CERRAR>"
// val1    : any int number
// operator: limited to {">", "<", "==", "!="}
// val2    : any int number
void calcOperationComponets(char* operation, Record* record);

Record* validateString(char* entry) {
    Record* result = newRecord();
    unsigned int lenght = strlen(entry);
    if (lenght < 17) return result;

    char* openTag = slice(entry, 0, 5);
    char* closeTag = slice(entry, lenght-6, lenght);

    if (strcmp(openTag, "ABRIR") && strcmp(closeTag, "CERRAR")) 
        return result;

    free(openTag);
    free(closeTag);

    char* asignation = slice(entry, 6, lenght-7);

    unsigned int asignationLen = strlen(asignation);
    if (asignationLen < 5) return result;

    unsigned int opIndex = 0;

    for (opIndex=0; opIndex<asignationLen; opIndex++) {
        if (asignation[opIndex] == '=') break;
    }

    char* varName = slice(asignation, 0, opIndex);
    char* operation = slice(asignation, opIndex+1, asignationLen);

    free(asignation);

    strcpy(result->name, varName); 

    free(varName);

    calcOperationComponets(operation, result);

    return result;
}

void calcOperationComponets(char* operation, Record* record) {
    unsigned int opStart;
    unsigned int doubleOp;
    for (opStart=0; opStart<strlen(operation); opStart++) {
        if (operation[opStart] == ' ') continue;
        if (
            operation[opStart] == '<' || operation[opStart] == '>' ||
            operation[opStart] == '=' || operation[opStart] == '!' 
        ) {
            doubleOp = operation[opStart+1] == '=' && operation[opStart] != '<' && operation[opStart] != '>';
            break;
        }
    }
    unsigned int opEnd = opStart+doubleOp+1;
    char* operator = slice(operation, opStart, opEnd);
    strcpy(record->operator, operator);

    free(operator);

    int val1 = atoi(slice(operation, 0, opStart));
    int val2 = atoi(slice(operation, opEnd, strlen(operation)));

    record->val1 = val1;
    record->val2 = val2;

    record->filled = 1;
}

void saveToFile(Record* record) {
    FILE *file = fopen("out.txt", "a+");
    fprintf(file, "%s\n", recordToStr(record));
    fclose(file);
}

int main() {
    char* entry = malloc(64*sizeof(char));
    printf(" > ");
    fgets(entry, 64, stdin);

    Record* out = validateString(trim(entry));
    if (!out->filled) {
        printf("Invalid\n");
        return 0;
    }

    printf("Valid\n");
    saveToFile(out);
    free(out);
    free(entry);
}