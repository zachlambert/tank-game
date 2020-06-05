
#include "entityUpdate.h"
#include <stdbool.h>
#include "mathDefines.h"

void updateTank(
    Entity* tank,
    double vx,
    double vy,
    double dt)
{
    // Update tank pose

    tank->data.pose.x += dt * vx;
    tank->data.pose.y += dt * vy;
    if(vx!=0 || vy!=0)
        tank->data.pose.angle = atan2(vy, vx);
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
    double angle = atan2(dy, dx);
    int index = (int)floor((angle+PI/8)/(PI/4));
    turret->data.pose.angle = index*PI/4;
}

void alignTurret(Entity* tank){
    // Sets the turret angle to the tank angle
    tank->children->data.pose.angle = tank->data.pose.angle;
}

int entityUpdatePlayer(Entity* player, Input* input, double dt)
{
    double vx = 0;
    double vy = 0;

    if(input->w && !input->s){
        vy = -player->data.tank.speed;
    }else if(input->s && !input->w){
        vy = player->data.tank.speed;
    }
    if(input->a && !input->d){
        vx = -player->data.tank.speed;
    }else if(input->d && !input->a){
        vx = player->data.tank.speed;
    }

    if(vx!=0 && vy!=0){
        vx/=sqrt(2);
        vy/=sqrt(2);
    }

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    updateTank(player, vx, vy, dt);
    pointTurret(player, (double)mx, (double)my, dt);
    return 0;
}

int entityUpdateDummy(Entity* entity, Input* input, double dt)
{
    static double dummyParam = 0;
    dummyParam += dt*0.2;
    if(dummyParam>=1) dummyParam=0;
    updateTank(
        entity,
        200*cos(dummyParam*2*PI),
        200*sin(dummyParam*2*PI),
        dt
    );
    alignTurret(entity);
    return 0;
}


