#ifndef SPRITES_H
#define SPRITES_H

#include <SDL2/SDL.h>

typedef enum Sprite {
    SPRITE_PLAYER_BASE,
    SPRITE_PLAYER_TURRET,
    SPRITE_COUNT // Use this to get the number of sprites
} Sprite;

typedef struct {
    const int FIRST_ROTATION; // Index of first sprite to be rotated
    const int NUM_ROTATIONS; // Number of rotations to pre-render
    SDL_Texture** textures; // Array of (pointers to) textures
} SpriteData;

SpriteData* initSpriteData(SDL_Renderer* renderer);

#endif
