#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdint.h>

// Organize Data
typedef struct _node 
{
    void *data;
    struct _node *next;
} Node;

typedef struct _linkedLidt 
{
    Node *head;
    Node *tail;
} LinkedList;

void ListInitialize(LinkedList *list);
void ListAddHead(LinkedList *list, void* data);
void ListAddTail(LinkedList *list, void *data);
void ListDeleteTail(LinkedList *list);
#endif

