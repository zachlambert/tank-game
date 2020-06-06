
#include "collisionResolve.h"

int moveOutOfCollision(Entity* entity){
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
    return 0;
}

int tankMoveOutOfCollision(Entity* entity){
    int result = moveOutOfCollision(entity);
    entity->children->data.pose.x = entity->data.pose.x;
    entity->children->data.pose.y = entity->data.pose.y;
    return result;
}
