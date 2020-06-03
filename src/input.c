
#include "input.h"
#include <SDL2/SDL.h>

void setKey(Input* input, SDL_KeyboardEvent* key, bool value){
    if(key->repeat != 0)
        return;
    switch(key->keysym.scancode)
    {
        case SDL_SCANCODE_W:
            input->w = value;
            break;
        case SDL_SCANCODE_S:
            input->s = value;
            break;
        case SDL_SCANCODE_A:
            input->a = value;
            break;
        case SDL_SCANCODE_D:
            input->d = value;
            break;
        default:
            break;
    }
}

void setMouse(Input* input, SDL_MouseButtonEvent* button, bool value){
    if(button->button == SDL_BUTTON_LEFT)
    {
        input->mouse = value;
    }
}

int doInput(Input* input)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
                // Stopped working for some reason. Maybe because I'm using
                // i3, where the exit is triggered by Ctrl-Shift-Key, which
                // also triggers key press events
				return 1;
            case SDL_KEYDOWN:
                if(((SDL_KeyboardEvent*)&event.key)->keysym.scancode
                        ==SDL_SCANCODE_ESCAPE)
                    return 1;
                setKey(input, &event.key, true);
                break;
            case SDL_KEYUP:
                setKey(input, &event.key, false);
                break;
            case SDL_MOUSEBUTTONDOWN:
                setMouse(input, &event.button, true);
                break;
            case SDL_MOUSEBUTTONUP:
                setMouse(input, &event.button, false);
                break;
            default:
                break;
		}
	}
    return 0;
}
