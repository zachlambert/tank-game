#ifndef ENTITY_DATA_H
#define ENTITY_DATA_H

#include "pose.h"
#include "sprite.h"
#include "input.h"

struct EntityDataTank {
    double linearSpeed;
    double rotateSpeed;
};

struct EntityDataTurret {
    double rotateSpeed;
};

struct Entity; // Forward declare, since all that is needed
typedef int (*EntityUpdate)(struct Entity*, Input*, double);

typedef struct {
    Pose pose;
    Sprite sprite;
    EntityUpdate update;
    union { // Unnamed union
        struct EntityDataTank tank;
        struct EntityDataTurret turret;
    };
} EntityData;

#endif
