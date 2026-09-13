#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

ListNode *appendInt(ListNode *list, int value)
{
    ListNode *t = (ListNode *)malloc(sizeof(ListNode));
    if (t == NULL)
    {
        return list;
    }
    t->type = TYPE_INT;
    t->data.ival = value;
    t->next = list;
    return t;
}

ListNode *appendStr(ListNode *list, char *value)
{
    ListNode *t = (ListNode *)malloc(sizeof(ListNode));
    if (t == NULL)
    {
        return list;
    }
    t->type = TYPE_STR;
    t->data.sval = strdup(value);
    t->next = list;
    return t;
}

ListNode *delete(ListNode *list, int index)
{
    ListNode *prev = NULL;
    ListNode *next = NULL;

    ListNode *t = list;

    while (t && (len(list) - index - 1 != 0))
    {
        prev = t;
        t = t->next;
        index++;
    }

    if (!t)
        return list;

    if (!prev)
    {
        list = t->next;
    }
    else
    {
        prev->next = t->next;
    }

    if (t->type == TYPE_STR)
        free(t->data.sval);
    free(t);
    return list;
}

int printList(ListNode *list)
{
    if (!list)
        return -1;

    printf("[");
    while (list)
    {
        if (list->type == TYPE_INT)
            printf("%d", list->data.ival);
        else
            printf("%s", list->data.sval);

        list = list->next;

        if (list)
        {
            printf(", ");
        }
    }

    printf("]\n");
    return 0;
}

int len(ListNode *list)
{
    if (list == NULL)
        return 0;
    int count = 0;

    while (list)
    {
        count++;
        list = list->next;
    }

    return count;
}

void clear(ListNode *list)
{
    while (!list)
    {
        ListNode *next = list->next;
        if (list->type == TYPE_STR)
            free(list->data.sval);
        free(list);
        list = next;
    }
}

// int main(void)
// {
//     ListNode *ln = NULL;

//     // Append to list 10 numbers
//     for (int i = 0; i < 10; i++)
//     {
//         ln = appendInt(ln, i);
//     }

//     // Append 5 strings
//     for (int i = 0; i < 5; i++)
//     {
//         ln = appendStr(ln, "a");
//     }

//     // Test Print
//     printList(ln);

//     // Test Append
//     ln = appendInt(ln, 67);
//     printList(ln);

//     // Test append string
//     ln = appendStr(ln, "b");
//     printList(ln);

//     // Test delete
//     ln = delete(ln, 0);
//     printList(ln);

//     printf("%d\n", len(ln));
//     return 0;
// }