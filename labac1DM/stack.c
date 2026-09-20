#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void push(StackNodePtr *top, int value)
{
    StackNodePtr newPtr = (StackNodePtr)malloc(sizeof(StackNode));
    if (newPtr != NULL)
    {
        newPtr->data = value;
        newPtr->nextPtr = *top;
        *top = newPtr;
    }
    else
    {
        printf("newPtr is NULL");
    }
}

int pop(StackNodePtr *top)
{
    if (*top == NULL)
    {
        return 0;
    }
    StackNodePtr CurrPtr = *top;
    int value = CurrPtr->data;
    *top = (*top)->nextPtr;
    free(CurrPtr);
    return value;
}

void printStack(StackNodePtr top)
{
    // printf("[");
    while (top)
    {
        StackNodePtr next = (top)->nextPtr;
        int data = (top)->data;
        if (next)
            printf("%d ", data);
        else
            printf("%d", data);

        top = next;
    }
    // printf("]\n");
}

int isEmpty(StackNodePtr top)
{
    return top == NULL;
}

int lenStack(StackNodePtr top)
{
    int n = 0;
    while (top)
    {
        n++;
        top = top->nextPtr;
    }
    return n;
}

char stackTop(StackNodePtr topPtr)
{
    if (topPtr == NULL)
    {
        return -1;
    }

    return topPtr->data;
}

// int main(void)
// {
// StackNodePtr stack = NULL;

// push(&stack, 10);
// push(&stack, 'a');
// printStack(stack);
// int n = lenStack(stack);
// printf("%d", n);
//     pop(&stack);
//     printStack(stack);

//     printf("\n");
//     // return 0;
// }