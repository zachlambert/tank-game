#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include "linkedList.h"
#include "input.h"

typedef enum {
    LAYER_ZERO, // Can rename if I want
    LAYER_ONE,
    LAYER_TWO,
    LAYER_COUNT
} Layer;

typedef struct {
    Entity* player;
    List** layers;
} World;

World* initWorld(void);
void insertEntity(World* world, Layer layer, Entity* entity);
void updateWorld(World* world, Input* input, double dt);

#endif
