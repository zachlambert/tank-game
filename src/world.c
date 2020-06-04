
#include "world.h"
#include "input.h"
#include "sprite.h"

#define PI 3.14159

void insertEntity(World* world, Layer layer, Entity* entity)
{
    insertList(world->layers + layer, entity);
}

World* initWorld(void)
{
    // Create world
    World* world = calloc(1, sizeof(World));
    
    // Create layers - Use calloc so they default to null pointers
    world->layers = calloc(LAYER_COUNT, sizeof(List*));

    // The player will be the first entity
    Entity* player = calloc(1, sizeof(Entity));
    player->pose.x = 500;
    player->pose.y = 300;
    player->sprite = SPRITE_PLAYER_BASE;
    player->update = entityUpdatePlayerTank;
    player->data.tank.forwardSpeed = 300;
    player->data.tank.backwardSpeed = 100;
    player->data.tank.rotateSpeed = 2.5;

    insertEntity(world, LAYER_ZERO, player);
    world->player = player; // Also store a pointer to the player

    // Add another entity, for the player turret
    Entity* turret = calloc(1, sizeof(Entity));
    turret->sprite = SPRITE_PLAYER_TURRET;
    turret->parent = player;
    turret->update = entityUpdatePlayerTurret;
    turret->data.turret.rotateSpeed = 2.5;
    insertEntity(world, LAYER_ONE, turret);

    return world;
}

void updateWorld(World* world, Input* input, double dt)
{
    // Iterate through entities on all layers
    List** stop = world->layers + LAYER_COUNT;
    List* list;
    Entity* entity;
    for(List** layer = world->layers; layer<stop; layer++){
        list = *layer;
        while(list!=NULL){
            entity = list->data;
            if(entity->update != NULL)
                entity->update(entity, input, dt);
            list = list->next;
        }
    }
}

