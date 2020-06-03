/*
Copyright (C) 2015-2018 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "input.h"

void setKey(Input* input, SDL_KeyboardEvent* key, int value){
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

void setMouse(Input* input, SDL_MouseButtonEvent* button, int value){
    if(button->button == SDL_BUTTON_LEFT)
    {
        input->mouse = value;
    }
}

int doInput(App* app)
{
	SDL_Event event;
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return 1;
            case SDL_KEYDOWN:
                setKey(app->input, &event.key, 1);
                break;
            case SDL_KEYUP:
                setKey(app->input, &event.key, 1);
                break;
            case SDL_MOUSEBUTTONDOWN:
                setMouse(app->input, &event.button, 1);
                break;
            case SDL_MOUSEBUTTONUP:
                setMouse(app->input, &event.button, 0);
                break;
            default:
                break;
		}
	}
    return 0;
}
