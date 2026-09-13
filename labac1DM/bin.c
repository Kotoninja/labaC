#include <stdio.h>
#include "list.h"
#include <math.h>

int convertToBin(int number)
{
    ListNode *digits = NULL;

    while (number != 0)
    {
        int remainder = number % 2;
        digits = appendInt(digits, remainder);
        number /= 2;
    }

    int binNumber = 0;
    int i = 0;
    int lenDigits = len(digits);
    while (digits)
    {
        binNumber += digits->data.ival * pow(10, lenDigits - 1 - i);
        i++;
        digits = digits->next;
    }
    return binNumber;
}

int main(void)
{
    {
        int number = convertToBin(25);
        int expectedNumber = 11001;
        printf("%d", number == expectedNumber);
    }

    {
        int number = convertToBin(42);
        int expectedNumber = 101010;
        printf("%d", number == expectedNumber);
    }

    {
        int number = convertToBin(5);
        int expectedNumber = 101;
        printf("%d", number == expectedNumber);
    }
    printf("\n");
}