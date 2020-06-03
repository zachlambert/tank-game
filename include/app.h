#ifndef APP_H
#define APP_H

#include "SDL2/SDL.h"

typedef struct {
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
	SDL_Renderer *renderer;
	SDL_Window *window;
} App;

#endif
