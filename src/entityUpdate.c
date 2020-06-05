
#include "entityUpdate.h"
#include <stdbool.h>
#include "mathDefines.h"

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

int entityUpdatePlayer(Entity* player, Input* input, double dt)
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
    pointTurret(player, (double)mx, (double)my, dt);
    return 0;
}

int entityUpdateDummy(Entity* entity, Input* input, double dt)
{
    updateTank(entity, 200, 2, dt);
    alignTurret(entity);
    return 0;
}


