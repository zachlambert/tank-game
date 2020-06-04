
#include "entityUpdate.h"
#include "mathDefines.h"

void updateTank(
    Entity* tank,
    double linearVelocity,
    double angularVelocity,
    double targetX, double targetY,
    double dt)
{
    // Update tank pose

    tank->data.pose.x += dt * linearVelocity * cos(tank->data.pose.angle);
    tank->data.pose.y += dt * linearVelocity * sin(tank->data.pose.angle);
    tank->data.pose.angle += dt * angularVelocity;
    while(tank->data.pose.angle<0) tank->data.pose.angle += 2*PI;
    while(tank->data.pose.angle>2*PI) tank->data.pose.angle -= 2*PI;

    // Update turret angle

    Entity* turret = tank->children;

    Pose absolute = addPose(tank->data.pose, turret->data.pose);
    double dx = targetX - absolute.x;
    double dy = targetY - absolute.y;
    double targetAngle = atan2(dy, dx);

    // Make angleDif go from -PI to PI
    double difAngle = targetAngle - absolute.angle;
    while(difAngle<-PI) difAngle += 2*PI;
    while(difAngle>PI) difAngle -= 2*PI;

    double changeAngle = dt * turret->data.turret.rotateSpeed;
    if(difAngle < 0) changeAngle = -changeAngle;

    if(fabs(difAngle) < fabs(changeAngle)){
        turret->data.pose.angle = targetAngle - tank->data.pose.angle;
        if(turret->data.pose.angle < 0)
            turret->data.pose.angle += 2*PI;
    }else{
        turret->data.pose.angle += changeAngle;
    }
}

int entityUpdatePlayer(Entity* player, Input* input, double dt)
{
    double linearVelocity = 0;
    double angularVelocity = 0;

    if(input->w && !input->s){
        linearVelocity = player->data.tank.forwardSpeed;
    }else if(input->s && !input->w){
        linearVelocity = - player->data.tank.backwardSpeed;
    }
    if(input->a && !input->d){
        angularVelocity = -player->data.tank.rotateSpeed;
    }else if(input->d && !input->a){
        angularVelocity = player->data.tank.rotateSpeed;
    }
    if(linearVelocity<0) angularVelocity = -angularVelocity;

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    updateTank(
        player,
        linearVelocity,
        angularVelocity,
        (double)mx, (double)my,
        dt
    );

    return 0;
}


