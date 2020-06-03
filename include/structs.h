#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    int x;
    int y;
    SDL_Texture* texture;
} Entity;

typedef struct {
    Entity* player;
} World;

typedef struct {
    bool mouse;
    bool w;
    bool s;
    bool a;
    bool d;
} Input;

typedef struct {
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
	SDL_Renderer* renderer;
	SDL_Window* window;
    World* world;
    Input* input;
} App;

#endif
