#ifndef ENTITY_H
#define ENTITY_H

#include "sprite.h"

typedef struct {
    double x;
    double y;
    double angle;
} Pose;

typedef struct Entity{
    Pose pose;
    Sprite sprite;
    struct Entity* parent; // For doing relative positions
} Entity;

#endif
