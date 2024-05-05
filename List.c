#include "List.h"
#include "string.h"
#include <stdlib.h>


void ListInitialize(LinkedList *list) 
{
    list->head = NULL;
    list->tail = NULL;
}

void ListAddHead(LinkedList *list, void* data) 
{
    Node *node = (Node*) malloc(sizeof(Node));
    node->data = data;
    if (list->head == NULL) 
	{
        list->tail = node;
        node->next = NULL;
    } 
	else
	{
        node->next = list->head;
    }
    list->head = node;
}


void ListAddTail(LinkedList *list, void *data) 
{
    Node *node = (Node*) malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    if (list->head == NULL) 
	{
        list->head = node;
    } 
	else 
	{
        list->tail->next = node;
    }
    list->tail = node;
}

void ListDeleteHead(LinkedList *list) 
{
    if (list->head != NULL) 
	{
        Node *temp = list->head;
        list->head = list->head->next;
        free(temp);
        if (list->head == NULL) 
		{
            list->tail = NULL;
        }
    }
}

void ListDeleteTail(LinkedList *list) 
{
    if (list->head != NULL)
	{
        Node *current = list->head;
        Node *prev;
        while (current->next != NULL)
		{
            prev = current;
            current = current->next;
        }
        free(current);
        if (prev != NULL) {
            prev->next = NULL;
            list->tail = prev;
        } else 
		{
            list->head = NULL;
            list->tail = NULL;
        }
    }
}

void ListDelete(LinkedList *list, Node *node) {
    if(node == list->head)
    {
        if(list->head->next == NULL) {
            list->head = list->tail = NULL;
        } else {
            list->head = list->head->next;
        }
    } else {
        Node *tmp = list->head;
        while(tmp != NULL && tmp->next != node) {
            tmp = tmp->next;
        }
        if(tmp != NULL)
        {
            tmp->next = node->next;
        }
    }

    free(node);
}




