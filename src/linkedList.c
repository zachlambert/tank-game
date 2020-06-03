
#include "linkedList.h"
#include <stdio.h>

void insertList(List** list, void* data)
{
    // Insert at the start, it's most efficient
    List* new_node = malloc(sizeof(List));
    new_node->data = data;
    new_node->next = *list;
    *list = new_node;
}

void deleteList(List** list, List* to_delete)
{
    // First check if the first item is to be deleted
    if(*list == to_delete)
    {
        *list = to_delete->next;
        free(to_delete);
        return;
    }
    // Else, iterate through the list
    List* pred = *list;
    while(pred!=NULL && pred->next !=NULL && pred->next != to_delete){
        pred = pred->next;
    }
    if(pred==NULL)
        return; // Gave an empty list, do nothing
    if(pred->next==NULL)
        return; // Not present, do nothing
    // Connect pred and to_delete->next, then free to_delete
    pred->next = to_delete->next;
    free(to_delete);
}
