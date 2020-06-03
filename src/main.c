
#include <stdio.h>

#include "common.h"
#include "init.h"
#include "input.h"
#include "draw.h"

void runApp(App* app){
	if(initSDL(app))
        return;

	while (!doInput()) // Returns 1 to exit, 0 to keep going
	{
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
