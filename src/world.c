
#include "world.h"
#include "input.h"
#include "sprite.h"
#include "entity.h"
#include "entityUpdate.h"

#define PI 3.14159

World* initWorld(void)
{
    // Create world
    World* world = calloc(1, sizeof(World));

    // The player will be the first entity
    EntityData player;
    player.pose.x = 500;
    player.pose.y = 300;
    player.sprite = SPRITE_PLAYER_BASE;
    player.update = entityUpdatePlayer;
    player.tank.forwardSpeed = 400;
    player.tank.backwardSpeed = 400;
    player.tank.rotateSpeed = 4;

    Entity* entity = insertEntity(&(world->entities), player);

    EntityData turret;
    turret.pose.x = 0;
    turret.sprite = SPRITE_PLAYER_TURRET;
    turret.turret.rotateSpeed = 2.5;

    insertChild(entity, turret);

    // Create a dummy tank, re-use data
    player.pose.x = 300;
    player.update = entityUpdateDummy;
    entity = insertEntity(&world->entities, player);
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
            entity->data.update(entity, input, dt);
        }
        entity = entity->next;
    }
}

