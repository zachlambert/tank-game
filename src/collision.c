
#include "collision.h"
#include "entity.h"
#include "level.h"
#include "world.h"
#include <stdbool.h>


void addCollision(
    Collision** prev,
    CollisionData* data)
{
    Collision* collision = malloc(sizeof(Collision));
    collision->data = *data;
    collision->next = *prev;
    *prev = collision;
}


void findLevelCollisions(
    Entity* entity,
    Level* level,
    Collision** collision)
{
    // Check it has a bounding circle
    if(entity->data.radius == 0) return;

    // Tiles covered by the bounding box
    int bx1, by1, bx2, by2;
    bx1 = floor((entity->data.pose.x - entity->data.radius) / level->tileWidth);
    by1 = floor((entity->data.pose.y - entity->data.radius) / level->tileHeight);
    bx2 = floor((entity->data.pose.x + entity->data.radius) / level->tileWidth);
    by2 = floor((entity->data.pose.y + entity->data.radius) / level->tileHeight);

    // Tile covered by entity centre
    int centreX, centreY;
    centreX = floor(entity->data.pose.x / level->tileWidth);
    centreY = floor(entity->data.pose.y / level->tileHeight);

    // Create a collisionData object to put data into
    CollisionData data;
    data.first = entity;
    data.second = NULL;

    //First, check the tiles aligned to the centre
    // Horizontal
    if(getLevelTile(level, bx1, centreY) != '0'){
        data.x = (bx1+1)*level->tileWidth;
        data.y = entity->data.pose.y;
        data.normalX = -1;
        data.normalY = 0;
        addCollision(collision, &data);
    }else if(getLevelTile(level, bx2, centreY) != '0'){
        data.x = bx2*level->tileWidth;
        data.y = entity->data.pose.y;
        data.normalX = 1;
        data.normalY = 0;
        addCollision(collision, &data);
    }
    // Vertical 
    if(getLevelTile(level, centreX, by1) != '0'){
        data.x = entity->data.pose.x;
        data.y = (by1+1)*level->tileHeight;
        data.normalX = 0;
        data.normalY = -1;
        addCollision(collision, &data);
    }else if(getLevelTile(level, centreX, by2) != '0'){
        data.x = entity->data.pose.x;
        data.y = by2*level->tileHeight;
        data.normalX = 0;
        data.normalY = 1;
        addCollision(collision, &data);
    }

    // Iterative over tiles
    double dist, dx, dy;
    int offsetX, offsetY; // For checking that neighbouring tiles are empty
    for(int y = by1; y<=by2; y++){
        for(int x = bx1; x<=bx2; x++){
            // Make sure the tile is in range
            if(x<0 || y<0 || x>=level->width || y>=level->height) continue;
            // Skip non-corner tiles
            if(x==centreX || y==centreY) continue;
            // Skip if the tile is empty
            if(getLevelTile(level, x, y) == '0') continue;

            // Otherwise, check for collision with this tile
            // It needs to be a corner, so the neighbouring tiles are empty
 
            if(x<centreX){ // Left
                data.x = (x+1)*level->tileWidth;
                offsetX=1;
            }else{ // Right
                data.x = x*level->tileWidth;
                offsetX=-1;
            }
            if(y<centreY){ // Above
                data.y = (y+1)*level->tileHeight;
                offsetY=1;
            }else{ // Below
                data.y = y*level->tileHeight;
                offsetY=-1;
            }
            if(getLevelTile(level, x+offsetX, y)!='0' ||
               getLevelTile(level, x, y+offsetY)!='0') continue;
            dx = data.x - entity->data.pose.x;
            dy = data.y - entity->data.pose.y;
            dist = hypot(dx, dy);
            if(dist < entity->data.radius){
                data.normalX = dx/dist;
                data.normalY = dy/dist;
                addCollision(collision, &data);
            }
        }
    }
}

bool checkEntityCollision(Entity* first, Entity* second, CollisionData* data){
    if(first->data.radius==0 || second->data.radius==0) return false;
    double deltaX = second->data.pose.x - first->data.pose.x;
    double deltaY = second->data.pose.y - first->data.pose.y;
    double dist = hypot(deltaX, deltaY);
    if(dist > first->data.radius + second->data.radius){
        return false;
    }else{
        double contactDist = (first->data.radius + dist - second->data.radius) / 2;
        data->x = first->data.pose.x + deltaX*(contactDist/dist);
        data->y = first->data.pose.y + deltaY*(contactDist/dist);
        data->normalX = deltaX/dist;
        data->normalY = deltaY/dist;
        return true;
    }
}

void clearEntityCollisions(Entity* entity){
    Collision* prev;
    while(entity->data.collision != NULL){
        prev = entity->data.collision;
        entity->data.collision = entity->data.collision->next;
        free(prev);
    }
}

void findCollisions(World* world)
{
    // Only looks at first-level entities for collision, other entities
    // are just cosmetic

    // Find entity-level collisions
    Entity* entity = world->entities;
    Collision* collision = NULL;
    int i=0;
    while(entity != NULL){
        findLevelCollisions(entity, world->level, &collision);
        entity = entity->next;
    }
    // Find entity-entity collisions
    Entity *first, *second;
    first = world->entities;
    CollisionData data;
    while(first != NULL){
        data.first = first;
        if(first->data.radius!=0){
            second = first->next;
            while(second != NULL){
                data.second = second;
                if(checkEntityCollision(first, second, &data)){
                    addCollision(&collision, &data);
                }
                second = second->next;
            }
        }
        first = first->next;
    }

    // Pass the collision information to the entities
    Collision* prev;
    while(collision != NULL){
        if(collision->data.first){
            addCollision(
                &collision->data.first->data.collision,
                &collision->data);
        }
        if(collision->data.second){
            Entity* temp = collision->data.first;
            collision->data.first = collision->data.second;
            collision->data.second = temp;
            collision->data.normalX*=-1;
            collision->data.normalY*=-1;
            addCollision(
                &collision->data.first->data.collision,
                &collision->data);
        }
        prev = collision;
        collision = collision->next;
        free(prev);
    }
}
