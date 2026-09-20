#include <stdio.h>
#include "stack.h"
#include <math.h>

StackNodePtr decimalToBinary(int number)
{
    StackNodePtr stack = NULL;

    while (number != 0)
    {
        int remainder = number % 2;
        push(&stack, remainder);
        number /= 2;
    }

    return stack;
}

StackNodePtr convertToBin(int number, int n)
{
    StackNodePtr stack = NULL;
    StackNodePtr binNumber = decimalToBinary(number);
    int difference = n - lenStack(binNumber);
    for (int i = 0; i < difference; i++)
    {
        push(&stack, 0);
    }

    while (binNumber)
    {
        push(&stack, pop(&binNumber));
    }

    return stack;
}

// int main(void)
// {
    // for (int i = 0; i <= 16; i++)
    // {
    //     convertToBin(i);
    // }
    // printf("\n");
// }