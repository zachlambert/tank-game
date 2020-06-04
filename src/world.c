
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
    player.tank.forwardSpeed = 300;
    player.tank.backwardSpeed = 100;
    player.tank.rotateSpeed = 2.5;

    Entity* playerEntity = insertEntity(&(world->entities), player);

    EntityData turret;
    turret.pose.x = 0;
    turret.sprite = SPRITE_PLAYER_TURRET;
    turret.turret.rotateSpeed = 2.5;

    insertChild(playerEntity, turret);

    // See what happens if I create some more players
    player.pose.x = 600;
    player.tank.forwardSpeed = 1000;
    player.tank.rotateSpeed = 10;
    playerEntity = insertEntity(&world->entities, player);
    insertChild(playerEntity, turret);

    player.pose.x = 200;
    player.pose.y = 400;
    player.tank.forwardSpeed = 100;
    player.tank.rotateSpeed = 20;
    playerEntity = insertEntity(&world->entities, player);
    turret.turret.rotateSpeed = 10;
    insertChild(playerEntity, turret);

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

