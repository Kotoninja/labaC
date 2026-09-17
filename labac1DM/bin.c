#include <stdio.h>
#include "stack.h"
#include <math.h>

StackNodePtr convertToBin(int number)
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

// int main(void)
// {
    // for (int i = 0; i <= 16; i++)
    // {
    //     convertToBin(i);
    // }
    // printf("\n");
// }