
#include "world.h"
#include "input.h"
#include "sprite.h"
#include "entity.h"
#include "entityUpdate.h"
#include "collision.h"
#include "resolveCollision.h"

#define PI 3.14159

Entity* insertTank(World* world, EntityData tank, EntityData turret)
{
    Entity* entity = insertEntity(&world->entities, tank);
    insertChild(entity, turret);
    return entity;
}

void createPlayer(World* world, double x, double y)
{
    EntityData tank;
    tank.pose.x = x;
    tank.pose.y = y;
    tank.sprite = SPRITE_TANK_BLUE_BASE;
    tank.update = entityUpdatePlayer;
    tank.resolveCollision = resolveCollisionTank;
    tank.tank.linearSpeed = 400;
    tank.tank.rotateSpeed = 5;
    tank.radius = 42;
    tank.type = TANK;
    tank.tank.team = 0;

    EntityData turret;
    turret.sprite = SPRITE_TANK_BLUE_TURRET;
    turret.turret.rotateSpeed = 8;
    turret.turret.length = 60;
    turret.turret.wait = 0.2;
    turret.type = TURRET;

    world->player = insertTank(world, tank, turret);
}

void createEnemy(
    World* world,
    double x, double y,
    double linearSpeed, double rotateSpeed, double wait,
    EntityUpdate entityUpdate,
    EntityDataPathDir dir,
    double length)
{
    EntityData tank;
    tank.pose.x = x;
    tank.pose.y = y;
    tank.sprite = SPRITE_TANK_RED_BASE;
    tank.update = entityUpdate;
    tank.resolveCollision = resolveCollisionTank;
    tank.tank.linearSpeed = linearSpeed;
    tank.tank.rotateSpeed = rotateSpeed;
    tank.radius = 42;
    tank.type = TANK;
    tank.tank.team = 1;
    tank.tank.path.coord = 0;
    tank.tank.path.state = -1;
    tank.tank.path.dir = dir;
    tank.tank.path.length = length;

    EntityData turret;
    turret.sprite = SPRITE_TANK_RED_TURRET;
    turret.turret.rotateSpeed = 8;
    turret.turret.length = 60;
    turret.turret.wait = wait;
    turret.type = TURRET;

    insertTank(world, tank, turret);
}


World* initWorld(Level* level)
{
    // Create world
    World* world = calloc(1, sizeof(World));

    // Set the level
    world->level = level;

    createPlayer(world, 200, 200);

    createEnemy(world, 1400, 400, 200, 5, 1, entityUpdateEnemyCircle, 0, 0);

    createEnemy(world, 1000, 425, 300, 5, 0.2, entityUpdateEnemyLine, PATH_DIR_LEFT, 900);
    createEnemy(world, 200, 775, 300, 5, 0.2, entityUpdateEnemyLine, PATH_DIR_RIGHT, 900);
    createEnemy(world, 1000, 1075, 300, 5, 0.2, entityUpdateEnemyLine, PATH_DIR_LEFT, 900);

    createEnemy(world, 3000, 600, 400, 5, 0.5, entityUpdateEnemyLine, PATH_DIR_UP, 900);

    // Set the camera position
    world->camera.x = 50;
    world->camera.y = 100;
    world->camera.zoom = 0.5;

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

void updateWorld(
    World* world, Input* input, double dt,
    const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
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
            if (toDelete == world->player) world->player = NULL;
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
            if (toDelete == world->player) world->player = NULL;
            deleteEntity(&world->entities, toDelete);
        }
    }

    // Update the camera
    // Follow the player, unless they are near the edge of the level
    if (world->player) {
        world->camera.x = world->player->data.pose.x - SCREEN_WIDTH/2 / world->camera.zoom;
        world->camera.y = world->player->data.pose.y - SCREEN_HEIGHT/2 / world->camera.zoom;
        double worldWidth = world->level->tileWidth*world->level->width;
        double worldHeight= world->level->tileHeight*world->level->height;
        if (world->player->data.pose.x < SCREEN_WIDTH/2 / world->camera.zoom) {
            world->camera.x = 0;
        } else if (worldWidth - world->player->data.pose.x < SCREEN_WIDTH/2 / world->camera.zoom) {
            world->camera.x = worldWidth - SCREEN_WIDTH / world->camera.zoom;
        }
        if (world->player->data.pose.y < SCREEN_HEIGHT/2 / world->camera.zoom) {
            world->camera.y = 0;
        } else if (worldHeight - world->player->data.pose.y < SCREEN_HEIGHT/2 / world->camera.zoom) {
            world->camera.y = worldHeight - SCREEN_HEIGHT / world->camera.zoom;
        }

    }
}

