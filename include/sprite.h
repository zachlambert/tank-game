#ifndef SPRITES_H
#define SPRITES_H

#include <SDL2/SDL.h>

typedef enum {
    SPRITE_NONE, // Can leave Sprite=0 to indicate no sprite
    SPRITE_LEVEL,
    SPRITE_TANK_BLUE_BASE,
    SPRITE_TANK_BLUE_TURRET,
    SPRITE_TANK_RED_BASE,
    SPRITE_TANK_RED_TURRET,
    SPRITE_BULLET_BLUE,
    SPRITE_BULLET_RED,
    SPRITE_COUNT // Use this to get the number of sprites
} Sprite;

#endif
