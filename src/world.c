
#include "world.h"
#include "input.h"
#include "sprite.h"

#define PI 3.14159

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
    double speed = 150;
    double angular_speed = PI;
    Pose* player_pose = &world->player->pose;
    if(input->w && !input->s){
        player_pose->x += dt * speed * cos(player_pose->angle);
        player_pose->y += dt * speed * sin(player_pose->angle);
    }else if(input->s && !input->w){
        player_pose->x -= dt * speed * cos(player_pose->angle);
        player_pose->y -= dt * speed * sin(player_pose->angle);
    }
    if(input->a && !input->d){
        player_pose->angle -= dt * angular_speed;
    }else if(input->d && !input->a){
        player_pose->angle += dt * angular_speed;
    }
    while(player_pose->angle<0) player_pose->angle += 2*PI;
    while(player_pose->angle>2*PI) player_pose->angle -= 2*PI;
}

