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
}

int pop(StackNodePtr *top)
{
    StackNodePtr CurrPtr = *top;
    int value = CurrPtr->data;
    *top = (*top)->nextPtr;
    free(CurrPtr);
    return value;
}

void printStack(StackNodePtr top)
{
    printf("[");
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
    printf("]\n");
}

int main(void)
{
    StackNodePtr stack = NULL;

    push(&stack, 10);
    push(&stack, 'a');
    printStack(stack);
    
    pop(&stack);
    printStack(stack);
    
    printf("\n") ;
    // return 0;

}