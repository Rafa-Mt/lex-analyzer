#include "structs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

Node* newNode() {
    Node* new = NULL;
    new = (Node*)malloc(sizeof(Node));
    return new;
}

Node* newOperand(int value) {
    Node* new = newNode();
    new->type = OPERAND;
    new->operand = value;
}

Node* newOperator(char symbol, int(*behavior)(int, int)) {
    Node* new = newNode();
    new->type = OPERATOR;
    new->operator = symbol;
    new->operation = behavior;
}

List* newEmptyList() {
    List* new = (List*)malloc(sizeof(List));
    new->head = NULL;
    new->tail = NULL;
}

List* newList(Node* head) {
    List* new = NULL; 
    new = newEmptyList();
    new->head = head;
    new->tail = head;
    return new;
}

void listPush(List* list, Node* new) {
    Node* copy = copyNode(new);

    if (list->tail == NULL && list->head == NULL) {
        list->head = copy; 
        list->tail = copy;
        return;
    }
    list->tail->next = copy;
    copy->prev = list->tail;
    list->tail = copy;
    copy->next = NULL;
}

Stack* newEmptyStack() {
    Stack* new = (Stack*)malloc(sizeof(Stack));
    new->head = NULL;
    new->tail = NULL;
}

Stack* newStack(Node* head) {
    Node* copy = copyNode(head);
    
    Stack* new = NULL; 
    new = newEmptyStack();
    copy->next = NULL;
    new->head = copy;
    new->tail = copy;
    return new;
}

void stackPush(Stack* stack, Node* new) {
    Node* copy = copyNode(new);
    if (stack->head == NULL & stack->tail == NULL) {
        stack->head = copy;
        stack->tail = copy;
        return;
    }
    copy->next = stack->head;
    stack->head = copy;
}

Node* stackPop(Stack* stack) {
    Node* oldHead = stack->head;
    if (stack->head->next != NULL)
        stack->head = stack->head->next;
    else stack->head = NULL;
    return oldHead;
}

int structLength(Node* head) {
    Node* actual = head;
    int lenght = 1;
    while (actual->next != NULL) {
        lenght++;
        actual = actual->next;
    }
    return lenght;
}

char* structToStr(Node* head) {
    if (head == NULL) return "";
    Node* actual = head;
    char* out = malloc(512*sizeof(char));
    strcat(out, "[ ");
    while(actual != NULL) {
        char* buffer = malloc(32*sizeof(char));
        if (actual->type == OPERAND)
            sprintf(buffer,"%i ", actual->operand);

        if (actual->type == OPERATOR)
            sprintf(buffer, "%c ", actual->operator);

        strcat(out, buffer);
        actual = actual->next;
    }
    strcat(out,"]\0");
    return out;
}


void structFree(Node* head) {
    Node* actual = head;
    while(actual != NULL) {
        Node* toDelete = actual;
        actual = actual->next;
        free(toDelete);
    }
    // free(stack);
}

Node* copyNode(Node* in) {
    Node* new = newNode();
    new->next = in->next;
    new->type = in->type;
    new->operand = in->operand;
    new->operator = in->operator;
    new->operation = in->operation;
}