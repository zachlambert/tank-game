
#include "entity.h"

// Need the address of the Entity*, so you can re-write it
Entity* insertEntity(Entity** entities, EntityData data)
{
    Entity* newEntity = calloc(1, sizeof(Entity));
    newEntity->data = data;
    newEntity->next = *entities;
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
    if(entity->prev == NULL){ // Must be the first item
        *entities = entity->next;
    }else{
        entity->prev->next = entity->next;
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
