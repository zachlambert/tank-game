
#include "world.h"
#include "input.h"
#include "sprite.h"

World* initWorld(void)
{
    // The player will be the first entity

    Entity* player = calloc(1, sizeof(Entity));
    player->pose.x = 100;
    player->pose.y = 100;
    player->sprite = SPRITE_PLAYER_BASE;

    List* entities = calloc(1, sizeof(List));
    entities->data = player;

    World* world = calloc(1, sizeof(World));
    world->player = player;
    world->entities = entities;

    // Add another entity, for the player turret
    Entity* turret = calloc(1, sizeof(Entity));
    turret->sprite = SPRITE_PLAYER_TURRET;
    turret->parent = player;

    // Insert into entities
    //  !! Need to use world->entities here, to re-write the address
    //  stored in app, not the local entities, which it was copied from
    insertList(&world->entities, turret); // Need to pass address of list

    return world;
}

void updateWorld(World* world, Input* input, double dt)
{
    if(input->w && !input->s){
        world->player->pose.y -= dt * 100;
    }else if(input->s && !input->w){
        world->player->pose.y += dt * 100;
    }
    if(input->a && !input->d){
        world->player->pose.x -= dt * 100;
    }else if(input->d && !input->a){
        world->player->pose.x += dt * 100;
    }
}

