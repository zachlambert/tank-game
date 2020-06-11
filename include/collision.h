#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>

typedef struct{
    struct Entity* first;
    struct Entity* second; // Leave null for level
    double x;
    double y;
    double normalX;
    double normalY;
}CollisionData;

typedef struct Collision{
    struct Collision* next;
    CollisionData data;
}Collision;

struct World; // Only include in the c file
void findCollisions(struct World* world);
void clearEntityCollisions(struct Entity* entity);
bool checkSight(struct World* world, struct Entity* a, struct Entity* b);

#endif
