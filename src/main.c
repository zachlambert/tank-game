
#include <stdio.h>

#include "common.h"
#include "init.h"
#include "input.h"
#include "draw.h"

int initWorld(App* app){
    // The player will be the first entity

    Entity* player = calloc(1, sizeof(Entity));
    player->pose.x = 100;
    player->pose.y = 100;
    player->texture = loadTexture(app, "graphics/player_base.png");
    if(player->texture == NULL)
        return 1;

    List* entities = calloc(1, sizeof(List));
    entities->data = player;

    World* world = calloc(1, sizeof(World));
    world->player = player;
    world->entities = entities;

    // Add another entity, for the player turret
    Entity* turret = calloc(1, sizeof(Entity));
    turret->texture = loadTexture(app, "graphics/player_turret.png");
    turret->parent = player;

    // Insert into entities
    //  !! Need to use world->entities here, to re-write the address
    //  stored in app, not the local entities, which it was copied from
    insertList(&world->entities, turret); // Need to pass address of list

    app->world = world;
    return 0;
}

void updateWorld(App* app, double dt)
{
    World* world = app->world;
    Input* input = app->input;

    if(input->w && !input->s){
        world->player->pose.y -= dt * 100;
    }else if(input->s && !input->w){
        world->player->pose.y += dt * 100;
    }

    if(input->a && !input->d){
        world->player->pose.x -= dt * 100;
    }else if(input->d && !input->a){
        world->player->pose.x += dt * 100;
    }
}

void runApp(App* app){
	if(initSDL(app))
        return;

    if(initWorld(app))
        return;

    Input* input = calloc(1, sizeof(Input));
    app->input = input;

    Entity* player = app->world->player;

    long now = SDL_GetPerformanceCounter();
    long prev = 0;
    double dt = 0;
    double timeScaleFactor = 1/(double)SDL_GetPerformanceFrequency();
	while (!doInput(app)) // Returns 1 to exit, 0 to keep going
	{
        prev = now;
        now = SDL_GetPerformanceCounter();
        dt = (double)(now-prev) * timeScaleFactor;
        updateWorld(app, dt);
		drawScene(app);
		SDL_Delay(16); // Wait an arbitrary(ish) number of milliseconds
        // All this does it cap the framerate at ~60
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
