#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include "linkedList.h"
#include "input.h"

typedef struct {
    Entity* entities;
    Entity* player; // Specific entity corresponding to player
} World;

World* initWorld(void);
void updateWorld(World* world, Input* input, double dt);

#endif
