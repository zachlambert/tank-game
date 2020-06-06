
#include "collisionResolve.h"

void moveOutOfCollision(Entity* entity){
    Collision* collision = entity->data.collision;
    while(collision){
        //todo
        collision = collision->next;
    }
}
