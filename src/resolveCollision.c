
#include "resolveCollision.h"

void moveOutOfCollisionSingle(Entity* entity, Collision* collision){
    double dx, dy, dist;
    dx = collision->data.x - entity->data.pose.x;
    dy = collision->data.y - entity->data.pose.y;
    dist = entity->data.radius - hypot(dx, dy);
    if(dist<entity->data.radius){
        entity->data.pose.x -= dist*collision->data.normalX;
        entity->data.pose.y -= dist*collision->data.normalY;
    }
}

void moveOutOfCollision(Entity* entity){
    Collision* collision = entity->data.collision;
    while(collision){
        moveOutOfCollisionSingle(entity, collision);
        collision = collision->next;
    }
}

int resolveCollisionTank(Entity* entity){
    moveOutOfCollision(entity);
    entity->children->data.pose.x = entity->data.pose.x;
    entity->children->data.pose.y = entity->data.pose.y;
    Collision* collision = entity->data.collision;
    while(collision){
        if(collision->data.second && collision->data.second->data.type==BULLET
            && collision->data.second->data.bullet.team != entity->data.tank.team){
            return 1;
        }
        collision = collision->next;
    }
    return 0;
}

int resolveCollisionBullet(Entity* entity){
    Collision* collision = entity->data.collision;
    if(collision){
        if(--(entity->data.bullet.bounces) >= 0){
            while(collision){
                if (collision->data.second
                    && collision->data.second->data.type == BULLET
                    && collision->data.second->data.bullet.team != entity->data.bullet.team){
                    return 1;
                }
                moveOutOfCollisionSingle(entity, collision);
                // Reflect velocity along the collision normal
                double dist =
                    entity->data.bullet.vx * collision->data.normalX
                    + entity->data.bullet.vy * collision->data.normalY;
                entity->data.bullet.vx -= 2*dist*collision->data.normalX;
                entity->data.bullet.vy -= 2*dist*collision->data.normalY;
                collision = collision->next;
            }
            entity->data.pose.angle = atan2(
                entity->data.bullet.vy, entity->data.bullet.vx
            );
            return 0;
        }
        return 1;
    }
    return 0;
}
