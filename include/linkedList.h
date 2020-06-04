#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

// Doubly linked, generic (uses void pointers)
typedef struct List{
    void* data;
    struct List* next;
    struct List* prev;
} List;

void insertList(List** list, void* data);
void deleteList(List** list, List* to_delete);

#endif
