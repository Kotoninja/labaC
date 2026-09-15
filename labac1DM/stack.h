#ifndef STACK_H
#define STACK_H

typedef struct StackNode
{
    int data;
    struct StackNode *nextPtr;
} StackNode;

typedef StackNode *StackNodePtr;

void push(StackNodePtr *top, int value);
int pop(StackNodePtr *top);
void printStack(StackNodePtr top);
int isEmpty(StackNodePtr top);
char stackTop(StackNodePtr topPtr);

#endif
