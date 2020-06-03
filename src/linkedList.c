
#include "linkedList.h"

void insertList(Node** list, void* data)
{
    // Insert at the start, it's most efficient
    Node* new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = *list;
    *list = new_node;
}

void deleteList(Node** list, Node* to_delete)
{
    // First check if the first item is to be deleted
    if(*list == to_delete)
    {
        *list = to_delete->next;
        free(to_delete);
        return;
    }
    // Else, iterate through the list
    Node* pred = *list;
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
