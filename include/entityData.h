#ifndef ENTITY_DATA_H
#define ENTITY_DATA_H

#include "pose.h"
#include "sprite.h"
#include "input.h"
#include "collision.h"

struct EntityDataTank {
    double linearSpeed;
    double rotateSpeed;
};

struct EntityDataTurret {
    double rotateSpeed;
};

struct Entity; // Forward declare only need the pointer
typedef int (*EntityUpdate)(struct Entity*, Input*, double);
typedef int (*EntityResolveCollision)(struct Entity*);

typedef struct {
    Pose pose;
    Pose prevPose;
    Sprite sprite;
    unsigned int radius;
    EntityUpdate update;
    union { // Unnamed union
        struct EntityDataTank tank;
        struct EntityDataTurret turret;
    };
    Collision* collision;
    EntityResolveCollision resolveCollision;
} EntityData;

#endif
