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
    int result;

} Record;

Record* newRecord() {
    Record* result;
    result->name = malloc(65*sizeof(char));
    result->operator = malloc(3*sizeof(char));
    result->val1 = 0;
    result->val2 = 0;
    result->filled = 0;
    result->result = 0;

    return result;
}

char* recordToStr(Record* record) {
    char* result = malloc(256*sizeof(char));
    sprintf(
        result,
        "{\n\tName: \"%s\"\n\tValue 1: %i\n\tOperator: \"%s\"\n\tValue 2: %i\n\tResult: \"%s\"\n}", 
        record->name, record->val1, 
        record->operator, record->val2,
        record->result ? "True" : "False"
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
int calcOperationResult(Record* record);

Record* validateString(char* entry) {
    Record* result = newRecord();
    unsigned int lenght = strlen(entry);
    // if (lenght < 17) return result;

    char* openTag = slice(entry, 0, 5);
    char* closeTag = slice(entry, lenght-6, lenght);

    if (strcmp(openTag, "ABRIR")) {
        printf("Error at parsing: Keyword \"ABRIR\" not found");
        return result;
    }

    if (strcmp(closeTag, "CERRAR")) {
        printf("Error at parsing: Keyword \"CERRAR\" not found");
        return result;
    }

    free(openTag);
    free(closeTag);

    if (entry[5] != ' ') {
        printf("Error at parsing: Expected space after \"ABRIR\" ");
        return result;
    }

    if (entry[lenght-7] != ' ') {
        printf("Error at parsing: Expected space after \"CERRAR\" ");
        return result;
    }

    char* asignation = slice(entry, 6, lenght-7);

    unsigned int asignationLen = strlen(asignation);
    // if (asignationLen < 5) {
    //     return result;
    // }

    unsigned int opIndex = 0;
    unsigned int foundOperator;

    for (opIndex=0; opIndex<asignationLen; opIndex++) {
        if (
            asignation[opIndex] == '=' &&
            asignation[opIndex+1] != '=' 
        ) {
            foundOperator = 1;
            break;
        }
    }

    if (opIndex == asignationLen-1) {
        printf("Error at parsing: Expected '=' \n");
        return result;
    }

    char* varName = slice(asignation, 0, opIndex);
    if (!isLetter(varName[0])) {
        printf("Error at parsing: Variable name must start with letter ");
        return result;
    }

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
        if (
            operation[opStart] == ' ' || 
            isNum(operation[opStart])
        ) continue;
        if (
            operation[opStart] == '<' || operation[opStart] == '>' ||
            operation[opStart] == '=' || operation[opStart] == '!' 
        ) {
            doubleOp = operation[opStart+1] == '=' && operation[opStart] != '<' && operation[opStart] != '>';
            break;
        }

        // Character found is not operator nor number
        printf("Error at parsing: Expected operator, got '%c'\n", operation[opStart]);
        return;

    }
    unsigned int opEnd = opStart+doubleOp+1;

    char* operator = slice(operation, opStart, opEnd);
    strcpy(record->operator, operator);

    free(operator);

    char* val1 = slice(operation, 0, opStart);
    char* val2 = slice(operation, opEnd, strlen(operation));

    if (!atoi(val1) && strcmp(val1, "0")) {
        printf("Error at parsing: Value 1 is not a number\n");
        return;
    }

    if (!atoi(val2) && strcmp(val2, "0")) {
        printf("Error at parsing: Value 2 is not a number\n");
        return;
    }

    record->val1 = atoi(val1);
    record->val2 = atoi(val2);

    calcOperationResult(record);

    record->filled = 1;
}

int calcOperationResult(Record* record) {
    int result = 0;
    if (!strcmp(record->operator, ">")) 
        result = record->val1 > record->val2;

    if (!strcmp(record->operator, "<")) 
        result = record->val1 < record->val2;
        
    if (!strcmp(record->operator, "==")) 
        result = record->val1 == record->val2;

    if (!strcmp(record->operator, "!=")) 
        result = record->val1 != record->val2;

    record->result = result;
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
    if (!out->filled) return 0;
    

    printf("Valid\nResult is: %s", out->result ? "True" : "False");
    saveToFile(out);
    free(out);
    // free(entry);
}