
#include "world.h"
#include "input.h"
#include "sprite.h"
#include "entity.h"
#include "entityUpdate.h"
#include "collision.h"

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
    player.tank.linearSpeed = 400;
    player.tank.rotateSpeed = 3;
    player.radius = 32;

    Entity* entity = insertEntity(&(world->entities), player);

    EntityData turret;
    turret.sprite = SPRITE_TANK_BLUE_TURRET;
    turret.turret.rotateSpeed = 8;

    insertChild(entity, turret);

    // Create a dummy tank, re-use data
    player.pose.x = 300;
    player.update = entityUpdateDummy;
    player.sprite = SPRITE_TANK_RED_BASE;
    entity = insertEntity(&world->entities, player);
    turret.sprite = SPRITE_TANK_RED_TURRET;
    insertChild(entity, turret);

    return world;
}

void updateWorld(World* world, Input* input, double dt)
{
    Entity* entity = world->entities;
    while(entity){
        if(entity->data.update){
            // Don't check for children
            // Children are updated by the specific update function
            entity->data.prevPose = entity->data.pose;
            entity->data.update(entity, input, dt);
            clearEntityCollisions(entity); // Clear any previous collisions
            findCollisions(world);
        }
        entity = entity->next;
    }
}

