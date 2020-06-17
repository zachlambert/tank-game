
#include "entityUpdate.h"
#include <stdbool.h>
#include "mathDefines.h"
#include "collision.h"
#include "resolveCollision.h"
#include "world.h"

void updateTank(
    Entity* tank,
    double linearVelocity,
    double angularVelocity,
    double dt)
{
    // Update tank pose
    tank->data.pose.x += dt * linearVelocity * cos(tank->data.pose.angle);
    tank->data.pose.y += dt * linearVelocity * sin(tank->data.pose.angle);
    tank->data.pose.angle += dt * angularVelocity;
    if(tank->data.pose.angle<-PI) tank->data.pose.angle+=2*PI;
    if(tank->data.pose.angle>PI) tank->data.pose.angle-=2*PI;

    tank->children->data.pose.x = tank->data.pose.x;
    tank->children->data.pose.y = tank->data.pose.y;
    
    if(tank->children->data.turret.timeout>0){
        tank->children->data.turret.timeout -= dt;
    }
}

void pointTurret(
    Entity* tank,
    double targetX, double targetY,
    double dt)
{
    Entity* turret = tank->children;
    double dx = targetX - turret->data.pose.x;
    double dy = targetY - turret->data.pose.y;
    turret->data.pose.angle = atan2(dy, dx);
}

void alignTurret(Entity* tank){
    // Sets the turret angle to the tank angle
    tank->children->data.pose.angle = tank->data.pose.angle;
}

void tankShootBullet(Entity* tank, World* world, Sprite sprite){
    if(tank->children->data.turret.timeout>0){
        return;
    }
    tank->children->data.turret.timeout =
        tank->children->data.turret.wait;
    EntityData bullet;
    bullet.pose = tank->children->data.pose; // Copy turret pose
    bullet.pose.x += tank->children->data.turret.length
                     * cos(bullet.pose.angle);
    bullet.pose.y += tank->children->data.turret.length
                     * sin(bullet.pose.angle);
    bullet.update = entityUpdateBullet;
    bullet.resolveCollision = resolveCollisionBullet;
    bullet.collision = NULL;
    bullet.sprite = sprite;
    bullet.bullet.bounces = 1;
    bullet.bullet.damage = 1;
    bullet.bullet.speed = 800;
    bullet.bullet.vx = bullet.bullet.speed * cos(bullet.pose.angle);
    bullet.bullet.vy = bullet.bullet.speed * sin(bullet.pose.angle);
    bullet.radius = 18;
    bullet.type = BULLET;
    bullet.bullet.team = tank->data.tank.team;
    insertEntity(&world->entities, bullet);
}

int entityUpdateBullet(
    Entity* entity, World* world, Input* input, double dt)
{
    entity->data.pose.x += entity->data.bullet.vx * dt;
    entity->data.pose.y += entity->data.bullet.vy * dt;
    return 0;
}


int entityUpdatePlayer(Entity* player, World* world, Input* input, double dt)
{
    double linearVelocity = 0;
    double angularVelocity = 0;

    if(input->w && !input->s){
        linearVelocity = player->data.tank.linearSpeed;
    }else if(input->s && !input->w){
        linearVelocity = -player->data.tank.linearSpeed;
    }
    if(input->a && !input->d){
        angularVelocity = -player->data.tank.rotateSpeed;
    }else if(input->d && !input->a){
        angularVelocity = player->data.tank.rotateSpeed;
    }

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    updateTank(player, linearVelocity, angularVelocity, dt);
    double targetX, targetY;
    targetX = world->camera.x + ((double)mx) / world->camera.zoom;
    targetY = world->camera.y + ((double)my) / world->camera.zoom;
    pointTurret(player, targetX, targetY, dt);

    // Shoot bullets
    if(input->mouse){
        tankShootBullet(player, world, SPRITE_BULLET_BLUE);
    }
    return 0;
}

int entityUpdateEnemyCircle(Entity* entity, World* world, Input* input, double dt)
{
    updateTank(entity, entity->data.tank.linearSpeed, entity->data.tank.rotateSpeed, dt);
    if (world->player) {
        if (checkSight(world, world->player, entity)) {
            pointTurret(
                entity,
                world->player->data.pose.x,
                world->player->data.pose.y,
                dt
            );
            tankShootBullet(entity, world, SPRITE_BULLET_RED);
            return 0;
        }
    }
    entity->children->data.pose.angle = entity->data.pose.angle;
    return 0;
}

int entityUpdateEnemyLine(Entity* entity, World* world, Input* input, double dt)
{
    EntityDataPath* path = &entity->data.tank.path;
    if (path->state == -1) { // Initial state
        entity->data.pose.angle = PI/2 * path->dir; 
        path->state = 0;
    }
    switch (path->state) {
    case 0:
        updateTank(entity, entity->data.tank.linearSpeed, 0, dt);
        path->coord += entity->data.tank.linearSpeed * dt;
        if (path->coord > path->length) {
            path->state = 1;
            path->coord = 0;
        }
        break;
    case 1:
        updateTank(entity, 0, entity->data.tank.rotateSpeed, dt);
        path->coord += entity->data.tank.rotateSpeed * dt;
        if (path->coord > PI) {
            path->state = 2;
            path->coord = 0;
            entity->data.pose.angle = PI/2 * (2 + path->dir);
        }
        break;
    case 2:
        updateTank(entity, entity->data.tank.linearSpeed, 0, dt);
        path->coord += entity->data.tank.linearSpeed * dt;
        if (path->coord > path->length) {
            path->state = 3;
            path->coord = 0;
        }
        break;
    case 3:
        updateTank(entity, 0, entity->data.tank.rotateSpeed, dt);
        path->coord += entity->data.tank.rotateSpeed * dt;
        if (path->coord > PI) {
            path->state = 0;
            path->coord = 0;
            entity->data.pose.angle = PI/2 * path->dir; 
        }
        break;
    default:
        break;
    }
    if (entity->data.pose.angle > PI) {
        entity->data.pose.angle -= 2*PI;
    }
    if (world->player) {
        if (checkSight(world, world->player, entity)) {
            pointTurret(
                entity,
                world->player->data.pose.x,
                world->player->data.pose.y,
                dt
            );
            tankShootBullet(entity, world, SPRITE_BULLET_RED);
            return 0;
        }
    }
    entity->children->data.pose.angle = entity->data.pose.angle;
    return 0;
}
