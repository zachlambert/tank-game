
#include "entity.h"

// Need the address of the Entity*, so you can re-write it
Entity* insertEntity(Entity** entities, EntityData data)
{
    Entity* newEntity = malloc(sizeof(Entity));
    newEntity->data = data;
    newEntity->data.collision = NULL; // Ensure collision starts at null
    newEntity->next = *entities;
    newEntity->prev = NULL;
    newEntity->children = NULL;
    if(*entities!=NULL){
        (*entities)->prev = newEntity;
    }
    // Used calloc, so other pointers default to 0
    *entities = newEntity;
    return newEntity;
}

Entity* insertChild(Entity* parent, EntityData childData)
{
    return insertEntity(&(parent->children), childData);
}

// Also deletes children
void deleteEntity(Entity** entities, Entity* entity)
{
    if(entity->next != NULL){
        entity->next->prev = entity->prev;
    }
    if(entity->prev != NULL){
        entity->prev->next = entity->next;
    }else{
        *entities = entity->next;
    }
    Entity* current = entity->children;
    Entity* next;
    while(current){
        next = current->next;
        free(current);
        current = next;
    }
    free(entity);
}
