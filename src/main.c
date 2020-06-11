
#include <stdio.h>

#include "app.h"

int main(int argc, char *argv[])
{
    // Initialise with { ... }
    // Omitted fields are initialised to zero
    App app_obj = {
        .SCREEN_WIDTH = 1600,
        .SCREEN_HEIGHT = 900
    };
    App* app = &app_obj;

    if(!initApp(app)) // Returns 0 if successful
    {
        runApp(app);
    }
	cleanupApp(app);

	return 0;
}
