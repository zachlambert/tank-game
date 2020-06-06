
#include "collisionResolve.h"

void moveOutOfCollision(Entity* entity){
    Collision* collision = entity->data.collision;
    while(collision){
        double dx, dy, dist;
        dx = collision->data.x - entity->data.pose.x;
        dy = collision->data.y - entity->data.pose.y;
        dist = entity->data.radius - hypot(dx, dy);
        if(dist<entity->data.radius){
            entity->data.pose.x -= dist*collision->data.normalX;
            entity->data.pose.y -= dist*collision->data.normalY;
        }
        collision = collision->next;
    }
}
