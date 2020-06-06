#ifndef COLLISION_H
#define COLLISION_H

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

#endif
