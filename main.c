#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "stringfuncs.h"
#include "structs.h"

typedef struct recordStruct {
    char* infix;
    char* postfix;
    int result;

} Record;

Record* newRecord() {
    Record* result;
    result->infix = malloc(512*sizeof(char));
    result->postfix = malloc(512*sizeof(char));
    result->result = 0;

    return result;
}

char* recordToStr(Record* record) {
    char* result = malloc(512*sizeof(char));
    sprintf(
        result,
        "{\n\tInfix Expression: %s\n\tPostfix Expression: %s\n\tResult: %i\n}", 
        record->infix, record->postfix, record->result
    );

    return result;
}

void printRecord(Record* record) {
    printf("Result: %s\n", recordToStr(record));
}

int sum(int a, int b) { return a+b; }
int subtract(int a, int b) { return a-b; }
int multiply(int a, int b) { return a*b; }
int power(int a, int b) {
    int result = 1;
    for (int t=1; t<=b; t++) {
        result *= a;
    } 
    return result; 
}

List* checkAndConvert(char* entry) {
    List* result = newEmptyList();

    unsigned int length = strlen(entry);
    // if (lenght < 17) return result;

    char* openTag = slice(entry, 0, 5);
    char* closeTag = slice(entry, length-6, length);

    if (strcmp(openTag, "ABRIR")) {
        printf("Error at parsing: Keyword \"ABRIR\" not found");
        return NULL;
    }

    if (strcmp(closeTag, "CERRAR")) {
        printf("Error at parsing: Keyword \"CERRAR\" not found");
        return NULL;
    }

    free(openTag);
    free(closeTag);

    if (entry[5] != ' ') {
        printf("Error at parsing: Expected space after \"ABRIR\" ");
        return result;
    }

    if (entry[length-7] != ' ') {
        printf("Error at parsing: Expected space after \"CERRAR\" ");
        return result;
    }

    char* operation = slice(entry, 6, length-7);
    int oplength = length-7-6;

    char operators[4] = "+-*^";
    int(*operations[4])(int, int) = {&sum, &subtract, &multiply, &power};

    for (int i=0; i<oplength-1; i++) {
        if (charIn(operation[i], operators) || isNum(operation[i]))  
            continue;
        
        printf("Error at parsing: Unexpected character '%c' %i\n", operation[i], i);
        return NULL;
    }

    int right = 0;
    int left = 0;
    for(int i=0; i<oplength; i++) {
        int isOperator = 0;
        for (int j=0; j<4; j++) {
            if (operation[i] == operators[j]) {
                char* subStr = slice(operation, left, right);
                int number = atoi(subStr);
                free(subStr);

                listPush(result, newOperand(number));
                listPush(result, newOperator(operators[j], operations[j]));

                left = i+1;
                right = i+1;
                isOperator = 1;
            }
        }
        if (isOperator) continue;

        if (i == oplength-1) {
            char* subStr = slice(operation, left, oplength);
            // printf("{%s}\n", subStr);
            int number = atoi(subStr);
            free(subStr);

            listPush(result, newOperand(number));
            continue;
        }

        right++;
    }

    free(operation);

    return result;
}
int getPriority(char entry) {
    if (entry == '+' || entry == '-') {
        return 1;
    }
    else if (entry == '*') return 2;
    else if (entry == '^') return 3;
}

int higherPriority(char a, char b) {
    return getPriority(a) > getPriority(b);
}

List* infixToPostfix(List* entry) {
    List* result = newList(newOperand(entry->head->operand));
    Stack* auxStack = newStack(entry->head->next);

    Node* actual = entry->head->next->next;
    while (actual != NULL) {
        if (actual->type == OPERAND) {
            listPush(result, actual);
            actual = actual->next;
            continue;
        }   

        int actualPriority = getPriority(actual->operator);
        if (getPriority(auxStack->head->operator) >= actualPriority) {
            while (getPriority(auxStack->head->operator) >= actualPriority) {
                Node* popped = stackPop(auxStack);
                listPush(result, popped);
                if (auxStack->head == NULL) break; 
            }
            stackPush(auxStack, actual);
        }
        else {
            stackPush(auxStack, actual);
        }
        actual = actual->next;
    }

    actual = auxStack->head;
    while (actual != NULL) {
        Node* popped = stackPop(auxStack);

        listPush(result, popped);
        actual = actual->next;
    }

    return result;
}

int solvePostfix(List* operation) {
    Node* actual = operation->head;
    Stack* auxStack = newEmptyStack();
    while (actual != NULL) {
        if (actual->type == OPERAND) {
            stackPush(auxStack, newOperand(actual->operand));   
        }
        else {
            // printf("%c ", actual->operator);
            Node* val2 = stackPop(auxStack);
            Node* val1 = stackPop(auxStack);
            int result = actual->operation(val1->operand, val2->operand);
            free(val1);
            free(val2);
            stackPush(auxStack, newOperand(result));

            if (auxStack->head == NULL) break;
        }
        actual = actual->next;
    }
    int solution = auxStack->head->operand;
    free(auxStack->head); 
    free(auxStack); 
    return solution;
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
    strcpy(entry, trim(entry));

    List* infix = checkAndConvert(entry);
    if (infix == NULL) {
        return 0;
    }

    char* instr = structToStr(infix->head);

    List* postfix = infixToPostfix(infix);
    char* poststr = structToStr(postfix->head);

    int solution = solvePostfix(postfix);

    Record* output = newRecord();
    output->infix = instr;
    output->postfix = poststr;
    output->result = solution;

    printRecord(output);
    saveToFile(output);

    free(entry);
    free(infix);
    free(postfix);
    free(poststr);
    return 0;
}   