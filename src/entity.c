
#include "entity.h"
#include "mathDefines.h"

void updateTurret(Entity* entity, double target, double dt)
{
    double absolute =
        entity->parent->pose.angle
        + entity->pose.angle;
    while(absolute<0) absolute += 2*PI;
    while(absolute>2*PI) absolute -= 2*PI;

    // Make angleDif go from -PI to PI
    double dif = target - absolute;
    while(dif<-PI) dif += 2*PI;
    while(dif>PI) dif -= 2*PI;

    double change = dt * entity->data.turret.rotateSpeed;
    if(dif < 0) change = -change;

    if(fabs(dif) < fabs(change)){
        entity->pose.angle = target - entity->parent->pose.angle;
        if(entity->pose.angle < 0)
            entity->pose.angle += 2*PI;
    }else{
        entity->pose.angle += change;
    }
}

void updateTank(Entity* entity,
                double linearVelocity,
                double angularVelocity,
                double dt)
{
    entity->pose.x += dt * linearVelocity * cos(entity->pose.angle);
    entity->pose.y += dt * linearVelocity * sin(entity->pose.angle);
    entity->pose.angle += dt * angularVelocity;
    while(entity->pose.angle<0) entity->pose.angle += 2*PI;
    while(entity->pose.angle>2*PI) entity->pose.angle -= 2*PI;
}

int entityUpdatePlayerTurret(Entity* entity, Input* input, double dt)
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    Pose absolute = addPose(entity->parent->pose, entity->pose);
    double dx = (double)mx - absolute.x;
    double dy = (double)my - absolute.y;
    double angleToMouse = atan2(dy, dx);
    updateTurret(entity, angleToMouse, dt);
    return 0;
}

int entityUpdatePlayerTank(Entity* entity, Input* input, double dt)
{
    double linearVelocity = 0;
    double angularVelocity = 0;

    if(input->w && !input->s){
        linearVelocity = entity->data.tank.forwardSpeed;
    }else if(input->s && !input->w){
        linearVelocity = - entity->data.tank.backwardSpeed;
    }
    if(input->a && !input->d){
        angularVelocity = -entity->data.tank.rotateSpeed;
    }else if(input->d && !input->a){
        angularVelocity = entity->data.tank.rotateSpeed;
    }
    if(linearVelocity<0) angularVelocity = -angularVelocity;

    updateTank(entity, linearVelocity, angularVelocity, dt);
    return 0;
}


