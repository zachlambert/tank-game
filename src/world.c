
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
    player->pose.x = 100;
    player->pose.y = 100;
    player->sprite = SPRITE_PLAYER_BASE;

    insertEntity(world, LAYER_ZERO, player);
    world->player = player; // Also store a pointer to the player

    // Add another entity, for the player turret
    Entity* turret = calloc(1, sizeof(Entity));
    turret->sprite = SPRITE_PLAYER_TURRET;
    turret->parent = player;
    insertEntity(world, LAYER_ONE, turret);

    return world;
}

void updateWorld(World* world, Input* input, double dt)
{
    double speed = 225;
    double angular_speed = PI;
    Pose* player_pose = &(world->player->pose);
    Pose* turret_pose = &((Entity*)(world->layers[1]->data))->pose;
    // Need a better way to access entities

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

    // Also get the mouse position, and move the turret towards
    // this by increasing its relative angle
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    double dx = (double)mx-player_pose->x;
    double dy = (double)my-player_pose->y;
    double angleToMouse = atan2(dy, dx);

    double turretAngle = player_pose->angle + turret_pose->angle;
    while(turretAngle<0) turretAngle += 2*PI;
    while(turretAngle>2*PI) turretAngle -= 2*PI;

    // Make angleDif go from -PI to PI
    double angleDif = angleToMouse - turretAngle;
    while(angleDif<-PI) angleDif += 2*PI;
    while(angleDif>PI) angleDif -= 2*PI;

    double angleChange = dt * PI*1.6;
    if(angleDif < 0){
        angleChange = -angleChange;
    }
    if(fabs(angleDif) < fabs(angleChange)){
        turret_pose->angle = angleToMouse - player_pose->angle;
        if(turret_pose->angle < 0){
            turret_pose->angle += 2*PI;
        }
    }else{
        turret_pose->angle += angleChange;
    }
}

