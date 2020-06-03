#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include "linkedList.h"
#include "input.h"

typedef struct {
    Entity* player;
    List* entities;
} World;

World* initWorld(void);
void updateWorld(World* world, Input* input, double dt);

#endif
