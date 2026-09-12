#ifndef LIST_H
#define LIST_H

typedef struct ListNode
{
    int data;
    struct ListNode *next;
} ListNode;

ListNode *append(ListNode *list, int value);
ListNode *delete(ListNode *list, int value);
int printList(ListNode *list);
int len(ListNode *list);

#endif // LIST_H