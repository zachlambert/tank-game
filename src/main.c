
#include <stdio.h>

#include "common.h"
#include "init.h"
#include "input.h"
#include "draw.h"

int initWorld(App* app){
    Entity* player = calloc(1, sizeof(Entity));
    player->x = 100;
    player->y = 100;
    player->texture = loadTexture(app, "graphics/player.png");
    if(player->texture == NULL)
        return 1;

    World* world = calloc(1, sizeof(World));
    world->player = player;

    app->world = world;
    return 0;
}

void runApp(App* app){
	if(initSDL(app))
        return;

    if(initWorld(app))
        return;

    Entity* player = app->world->player;
	while (!doInput()) // Returns 1 to exit, 0 to keep going
	{
        blit(app, player->texture, player->x, player->y);
		drawScene(app);
		SDL_Delay(16); // Wait a number of milliseconds
	}
}

int main(int argc, char *argv[])
{
    // Initialise with { ... }
    // Omitted fields are initialised to zero
    App app_obj = {
        .SCREEN_WIDTH = 1280,
        .SCREEN_HEIGHT = 720
    };
    App* app = &app_obj;

    runApp(app);
	cleanup(app);

	return 0;
}
