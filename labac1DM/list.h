#ifndef LIST_H
#define LIST_H

typedef enum
{
    TYPE_INT,
    TYPE_STR
} Datatype;

typedef struct ListNode
{
    Datatype type;
    union
    {
        int ival;
        char *sval;
    } data;
    struct ListNode *next;
} ListNode;

ListNode *appendInt(ListNode *list, int value);
ListNode *appendSte(ListNode *list, char value);
ListNode *delete (ListNode *list, int value);
int printList(ListNode *list);
int len(ListNode *list);

#endif