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
    double length;
    double timeout;
    double wait;
};

struct EntityDataBullet {
    double speed;
    double damage;
    int bounces;
    double vx;
    double vy;
};

typedef enum{
    TANK,
    TURRET,
    BULLET
}EntityType;

struct Entity; // Forward declare only need the pointer
struct World;
typedef int (*EntityUpdate)(struct Entity*, struct World*, Input*, double);
typedef int (*EntityResolveCollision)(struct Entity*);

typedef struct {
    Pose pose;
    Sprite sprite;
    unsigned int radius;
    EntityUpdate update;
    EntityType type;
    union { // Unnamed union
        struct EntityDataTank tank;
        struct EntityDataTurret turret;
        struct EntityDataBullet bullet;
    };
    Collision* collision;
    EntityResolveCollision resolveCollision;
} EntityData;

#endif
