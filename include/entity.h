#ifndef ENTITY_H
#define ENTITY_H

#include "sprite.h"
#include "input.h"

struct EntityDataTank {
    double forwardSpeed;
    double backwardSpeed;
    double rotateSpeed;
};
struct EntityDataTurret {
    double rotateSpeed;
};

typedef union {
    struct EntityDataTank tank;
    struct EntityDataTurret turret;
} EntityData;

typedef struct {
    double x;
    double y;
    double angle;
} Pose;

// Forward declare to use in function pointer
struct Entity;
typedef int (*EntityUpdate)(struct Entity*, Input*, double);

typedef struct Entity{
    Pose pose;
    Sprite sprite;
    struct Entity* parent; // For doing relative positions
    EntityUpdate update;
    EntityData data;
} Entity;

int entityUpdatePlayerTurret(Entity*, Input*, double);
int entityUpdatePlayerTank(Entity*, Input*, double);

#endif
