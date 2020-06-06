#ifndef ENTITY_UPDATE_H
#define ENTITY_UPDATE_H

#include "entity.h"
#include "input.h"

struct World;
int entityUpdatePlayer(Entity*, struct World*, Input*, double);
int entityUpdateDummy(Entity*, struct World*, Input*, double);
int entityUpdateBullet(Entity*, struct World*, Input*, double);

#endif
