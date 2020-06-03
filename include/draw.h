#ifndef DRAW_H
#define DRAW_H

#include "common.h"

SDL_Texture* loadTexture(App* app, char* filename);
void blit(App* app, SDL_Texture *texture, int x, int y);
void drawScene(App* app);

#endif
