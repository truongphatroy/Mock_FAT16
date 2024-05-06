#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdint.h>

// Organize Data
typedef struct _node 
{
    uint32_t data;
    struct _node *next;
} Node;

typedef struct _linkedLidt 
{
    Node *head;
    Node *tail;
} LinkedList;

void ListInitialize(LinkedList *list);
void ListAddHead(LinkedList *list, uint32_t data);
void ListAddTail(LinkedList *list, uint32_t data);
void ListDeleteTail(LinkedList *list);
void ListDeleteAll(LinkedList *list);
#endif

