
#include "world.h"
#include "input.h"
#include "sprite.h"
#include "entity.h"
#include "entityUpdate.h"
#include "collision.h"
#include "resolveCollision.h"

#define PI 3.14159

World* initWorld(Level* level)
{
    // Create world
    World* world = calloc(1, sizeof(World));

    // Set the level
    world->level = level;

    // The player will be the first entity
    EntityData player;
    player.pose.x = 500;
    player.pose.y = 300;
    player.sprite = SPRITE_TANK_BLUE_BASE;
    player.update = entityUpdatePlayer;
    player.resolveCollision = resolveCollisionTank;
    player.tank.linearSpeed = 400;
    player.tank.rotateSpeed = 5;
    player.radius = 32;

    Entity* entity = insertEntity(&(world->entities), player);

    // Also set this to the player
    world->player = entity;

    EntityData turret;
    turret.sprite = SPRITE_TANK_BLUE_TURRET;
    turret.turret.rotateSpeed = 8;
    turret.turret.length = 60;
    turret.turret.wait = 0.2;

    insertChild(entity, turret);

    // Create a dummy tank, re-use data
    player.pose.x = 300;
    player.update = entityUpdateDummy;
    player.sprite = SPRITE_TANK_RED_BASE;
    entity = insertEntity(&world->entities, player);
    turret.sprite = SPRITE_TANK_RED_TURRET;
    turret.turret.wait = 1;
    insertChild(entity, turret);

    return world;
}

bool entityOutsideLevel(Entity* entity, Level* level){
    const static unsigned int allowance = 2;
    bool result = false;
    result |= entity->data.pose.x <
        -(float)allowance*level->tileWidth;
    result |= entity->data.pose.x >
        (float)(level->width+allowance)*level->tileWidth;
    result |= entity->data.pose.y <
        -(float)allowance*level->tileHeight;
    result |= entity->data.pose.y >
        (float)(level->height+allowance)*level->tileHeight;
    return result;
}

void updateWorld(World* world, Input* input, double dt)
{
    Entity* entity = world->entities;
    Entity* toDelete;
    int result;
    while(entity){
        toDelete = NULL;
        if(entity->data.update){
            // Don't check for children
            // Children are updated by the specific update function
            if(entity->data.update(entity, world, input, dt) ||
               entityOutsideLevel(entity, world->level))
            {
                toDelete = entity;
            }
        }
        entity = entity->next;
        if(toDelete){
            deleteEntity(&world->entities, toDelete);
        }
    }
    findCollisions(world);
    entity = world->entities;
    while(entity){
        toDelete = NULL;
        if(entity->data.resolveCollision){
            if(entity->data.resolveCollision(entity)){
                toDelete = entity;
            }
        }
        clearEntityCollisions(entity); // Clear any previous collisions
        entity = entity->next;
        if(toDelete){
            deleteEntity(&world->entities, toDelete);
        }
    }
}

