#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "world.h"
#include "input.h"
#include "sprite.h"

typedef struct {
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
	SDL_Renderer* renderer;
	SDL_Window* window;
    World* world;
    Input* input;
    SpriteData* spriteData;
} App;

int initApp(App* app);
void runApp(App* app);
void cleanupApp(App* app);

#endif
