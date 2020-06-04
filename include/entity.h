#ifndef ENTITY_H
#define ENTITY_H

#include "entityData.h"

// Entity itself is a node of a doubly-linked list
// Each entity can also have children, also a doubly-linked list
typedef struct Entity{
    // Tree items
    struct Entity* prev;
    struct Entity* next;
    struct Entity* children;
    // Data
    EntityData data;
    // The data is completely static, so you don't have to
    // worry about allocating memory to it, or freeing it with
    // the entity
} Entity;

Entity* insertEntity(Entity** entities, EntityData data);
Entity* insertChild(Entity* parent, EntityData childData);
void deleteEntity(Entity** entities, Entity* entity);

#endif
