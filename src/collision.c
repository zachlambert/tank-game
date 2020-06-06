
#include "collision.h"
#include "level.h"
#include <stdbool.h>

typedef struct{
    Entity* first;
    Entity* second; // Leave null for level
    double x;
    double y;
    double normalX;
    double normalY;
}CollisionData;

typedef struct Collision{
    struct Collision* next;
    CollisionData data;
}Collision;

void addCollision(
    Collision** prev,
    CollisionData* data)
{
    Collision* collision = malloc(sizeof(Collision));
    collision->data = *data;
    if(*prev != NULL){
        (*prev)->next = collision;
    }else{
        *prev = collision;
    }
}

bool checkTileCollision(
    Entity* entity,
    Level* level,
    int x, int y,
    CollisionData* data)
{
    // Set data to default values
    data->x = entity->data.pose.x;
    data->y = entity->data.pose.y;
    data->normalX = 0;
    data->normalY = 0;
    // Find tile covered by entity centre 
    int centreX, centreY;
    centreX = floor(entity->data.pose.x / level->tileWidth);
    centreY = floor(entity->data.pose.y / level->tileHeight);
    //Collision with axis-aligned edges
    if(x == centreX || y == centreY){
        if(x < centreX){ // Left
            data->x -= entity->data.radius;
            data->normalX = -1;
        }else if(x > centreX){ // Right
            data->x += entity->data.radius;
            data->normalX = 1;
        }else if(y < centreY){ // Above
            data->y -= entity->data.radius;
            data->normalY = -1;
        }else if(y > centreY){ // Below
            data->y += entity->data.radius;
            data->normalY = 1;
        }else{ // Inside a tile
            // Leave collision point at entity centre
            data->normalX =
                (double)(x*level->tileWidth + level->tileWidth/2)
                - entity->data.pose.x;
            data->normalY =
                (double)(y*level->tileHeight + level->tileHeight/2)
                - entity->data.pose.y;
            // Normalise
            double mag = hypot(data->normalX, data->normalY);
            data->normalX/=mag;
            data->normalY/=mag;
        }
        return true;
    }else{
        // Check if the nearest corner is within range
        double cornerX, cornerY;
        if(x<centreX){ // Left
            cornerX = x*level->tileWidth;
        }else{ // Right
            cornerX = (x+1)*level->tileWidth;
        }
        if(y<centreY){ // Left
            cornerY = y*level->tileHeight;
        }else{ // Right
            cornerY = (y+1)*level->tileHeight;
        }
        double dist = hypot(
            cornerX - entity->data.pose.x,
            cornerY - entity->data.pose.y);
        return (dist < entity->data.radius);
    }
}

void findCollisionEntityLevel(
    Entity* entity,
    Level* level,
    Collision** collision)
{
    // Check it has a bounding circle
    if(entity->data.radius == 0) return;
    // Tiles covered by the bounding box
    int bx1, by1, bx2, by2;
    bx1 = floor((entity->data.pose.x - entity->data.radius) / level->tileWidth);
    by1 = floor((entity->data.pose.y - entity->data.radius) / level->tileWidth);
    bx2 = floor((entity->data.pose.x + entity->data.radius) / level->tileWidth);
    by2 = floor((entity->data.pose.y + entity->data.radius) / level->tileWidth);

    // Create a collisionData object to put data into
    CollisionData data;
    data.first = entity;
    data.second = NULL;
    // Iterative over tiles
    for(int y = by1; y<=by2; y++){
        for(int x = bx1; x<=bx2; x++){
            // Make sure the tile is in range
            if(x<0 || y<0 || x>=level->width || y>=level->height) continue;
            // Skip if the tile is empty
            if(level->data[y*level->width + x] == '0') continue;
            // Otherwise, check for collision with this tile
            if(checkTileCollision(entity, level, x, y, &data)){
                addCollision(collision, &data);
            }
        }
    }
}

bool checkEntityCollision(Entity* first, Entity* second, CollisionData* data){
    double deltaX = second->data.pose.x - first->data.pose.x;
    double deltaY = second->data.pose.y - first->data.pose.y;
    double dist = hypot(deltaX, deltaY);
    if(dist > first->data.radius + second->data.radius){
        return false;
    }else{
        data->x = first->data.pose.x + deltaX*(first->data.radius/dist);
        data->y = first->data.pose.y + deltaY*(first->data.radius/dist);
        data->normalX = deltaX/dist;
        data->normalY = deltaY/dist;
        return true;
    }
}

void resolveCollisions(World* world)
{
    // Only looks at first-level entities for collision, other entities
    // are just cosmetic

    // Find entity-level collisions
    Entity* entity = world->entities;
    Collision* collision = NULL;
    while(entity != NULL){
        findCollisionEntityLevel(entity, world->level, &collision);
        entity = entity->next;
    }
    // Find entity-entity collisions
    Entity *first, *second;
    first = world->entities;
    CollisionData data;
    while(first != NULL){
        data.first = first;
        second = first->next;
        while(second != NULL){
            data.second = second;
            if(checkEntityCollision(first, second, &data)){
                addCollision(&collision, &data);
            }
            second = second->next;
        }
        first = first->next;
    }


    // Resolve the collisions
    Collision* prev;
    if(collision != NULL){
        // Process the collision: todo
        printf(
            "ENTITY-%s (%f, %f) norm (%f, %f)\n",
            (collision->data.second?"ENTITY":"TILE"),
            collision->data.x, collision->data.y,
            collision->data.normalX, collision->data.normalY
        );
        prev = collision;
        collision = collision->next;
        free(prev);
    }
}
