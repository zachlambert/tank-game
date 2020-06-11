#ifndef WORLD_H
#define WORLD_H

#include "entity.h"
#include "linkedList.h"
#include "input.h"
#include "level.h"

typedef struct{
    double x;
    double y;
    double zoom;
} Camera;

typedef struct World{
    Entity* entities;
    Entity* player; // Specific entity corresponding to player
    Level* level;
    Camera camera;
} World;

World* initWorld(Level* level);
void updateWorld(
    World* world, Input* input, double dt,
    const int SCREEN_WIDTH, const int SCREEN_HEIGHT
);

#endif
