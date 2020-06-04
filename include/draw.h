#ifndef DRAW_H
#define DRAW_H

# include <SDL2/SDL.h>
#include "world.h"

void drawScene(SDL_Renderer* renderer, World* world, SpriteData* spriteData);

#endif
