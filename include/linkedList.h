#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

// Singly linked, generic (uses void pointers)
typedef struct Node{
    void* data;
    struct Node* next;
} Node;

void insertList(Node** list, void* data);
void deleteList(Node** list, Node* to_delete);

#endif
