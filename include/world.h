#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include "linkedList.h"
#include "input.h"
#include "level.h"

typedef struct {
    Entity* entities;
    Entity* player; // Specific entity corresponding to player
    Level* level;
} World;

World* initWorld(Level* level);
void updateWorld(World* world, Input* input, double dt);

#endif
