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
    printList(digits);
    while (digits)
    {
        binNumber += digits->data.ival * pow(10, lenDigits -1- i);
        i++;
        digits = digits->next;
    }
    return binNumber;
}

// int main(void)
// {
//     int number = convertToBin(25);
//     printf("%d\n", number);
// }