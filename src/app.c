
#include "app.h"

#include <SDL_image.h>

#include "sprite.h"
#include "world.h"
#include "draw.h"

int initApp(App* app)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;
	windowFlags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

    // Create window
	app->window = SDL_CreateWindow(
        "Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        app->SCREEN_WIDTH, app->SCREEN_HEIGHT, windowFlags);

	if (!app->window)
	{
		printf("Failed to open %d x %d window: %s\n",
                app->SCREEN_WIDTH, app->SCREEN_HEIGHT, SDL_GetError());
		return 1;
	}

    // Create renderer
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);
	if (!app->renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		return 1;
	}

    // Initialise images
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    // Load textures, get sprite data

    app->spriteData = initSpriteData(app->renderer);
    if (!app->spriteData)
    {
        printf("Failed to load sprites.\n");
        return 1;
    }

    app->world = initWorld();
    if(!app->world)
        return 1;

    return 0;
}

void runApp(App* app){
	if(initApp(app))
        return;

    Input* input = calloc(1, sizeof(Input));
    app->input = input;

    Entity* player = app->world->player;

    long now = SDL_GetPerformanceCounter();
    long prev = 0;
    double dt = 0;
    double timeScaleFactor = 1/(double)SDL_GetPerformanceFrequency();
	while (!doInput(app->input)) // Returns 1 to exit, 0 to keep going
	{
        prev = now;
        now = SDL_GetPerformanceCounter();
        dt = (double)(now-prev) * timeScaleFactor;
        updateWorld(app->world, app->input, dt);
		drawScene(app->renderer, app->world, app->spriteData);
		SDL_Delay(16); // Wait an arbitrary(ish) number of milliseconds
        // All this does it cap the framerate at ~60
	}
}

void cleanupApp(App* app)
{
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);

    // Could unallocate the memory for the other objects within App,
    // but it doesn't really matter since we are at the end of the
    // program anyway.

	SDL_Quit();
}
