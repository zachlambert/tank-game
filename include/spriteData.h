#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H

#include <SDL2/SDL.h>
#include "level.h"

typedef struct {
    const int FIRST_ROTATION; // Index of first sprite to be rotated
    const int NUM_ROTATIONS; // Number of rotations to pre-render
    SDL_Texture** textures; // Array of (pointers to) textures
} SpriteData;

SpriteData* initSpriteData(SDL_Renderer* renderer, Level* level);

#endif
