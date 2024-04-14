#ifndef STRUCTS_H
#define STRUCTS_H

enum NodeTypes {
    OPERAND,
    OPERATOR
};

struct Node;
typedef struct Node {
    struct Node* next;
    struct Node* prev;
    int type;

    int operand; // Null if type == OPERATOR
    char operator; // Null if type == OPERAND
    int (*operation)(int, int); // Null if type == OPERAND
} Node;

typedef struct List {
    Node* head;
    Node* tail;
} List;

typedef struct Stack {
    Node* head;
    Node* tail;
} Stack;

Node* newOperand(int value);
Node* newOperator(char symbol, int(*behavior)(int, int));
Node* copyNode(Node* in);

List* newEmptyList();
List* newList(Node* head);
void listPush(List* list, Node* new);

Stack* newEmptyStack();
Stack* newStack(Node* head);
void stackPush(Stack* stack, Node* new);
Node* stackPop(Stack* stack);

int structLength(Node* head);
char* structToStr(Node* head);
void structFree(Node* head);

#endif 