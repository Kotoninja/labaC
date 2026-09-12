#include <stdio.h>
#include <stdlib.h>
#include "list.h"

ListNode *append(ListNode *list, int value)
{
    ListNode *t = (ListNode *)malloc(sizeof(ListNode));
    t->data = value;
    t->next = list;
    return t;
}

ListNode *delete(ListNode *list, int value)
{
    ListNode *prev = NULL;
    ListNode *next = NULL;

    ListNode *t = list;

    while (t && (t->data != value))
    {
        prev = t;
        t = t->next;
    }

    if (!t)
        return list;

    if (!prev)
    {
        next = t->next;
        free(t);
        return next;
    }
    else
    {
        prev->next = t->next;
        free(t);
        return list;
    }
}

int printList(ListNode *list)
{
    if (!list)
        return -1;

    printf("[");
    while (list)
    {
        printf("%c", list->data);

        list = list->next;

        if (list) {
            printf(", ");
        }
    }

    printf("]\n");
    return 0;
}

// int main(void)
// {

//     ListNode *ln = NULL;

//     for (int i = 0; i < 10; i++)
//     {
//         ln = append(ln, i);
//     }

//     ListNode *t = ln;
//     printList(ln);

//     ln = delete(ln, 5);
//     printList(ln);

//     printf("\n");
//     return 0;
// }