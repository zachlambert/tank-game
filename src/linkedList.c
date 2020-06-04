
#include "linkedList.h"
#include <stdio.h>

void insertList(List** list, void* data)
{
    // Insert at the start, it's most efficient
    List* new_node = malloc(sizeof(List));
    new_node->data = data;
    new_node->next = *list;
    new_node->prev = 0; // Used malloc, so not guaranteed to be empty
    *list = new_node;
}

void deleteList(List** list, List* to_delete)
{
    if(to_delete->prev == NULL){
        *list = to_delete->next;
    }else{
        to_delete->prev->next = to_delete->next;
    }
    free(to_delete);
}
