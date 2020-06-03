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

#include "draw.h"
#include <SDL_log.h>

SDL_Texture* loadTexture(App* app, char* filename)
{
    SDL_Texture* texture;
    SDL_LogMessage(
        SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
        "Loading %s", filename);
    texture = IMG_LoadTexture(app->renderer, filename);
    if(texture == NULL)
    {
        SDL_LogMessage(
            SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
            "Failed to load %s", filename);
    }
    return texture;
}

void blit(App* app, SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(app->renderer, texture, NULL, &dest);
    // Draw texture onto renderer at rectangle dst.
    // NULL is passed as the src rect to indicate the whole image.
}

Pose accumulatePose(Entity* entity){
    Pose pose = entity->pose; // Copy
    while(entity->parent != NULL){
        entity = entity->parent;
        pose.x += entity->pose.x;
        pose.y += entity->pose.y;
        pose.angle += entity->pose.angle;
    }
    while(pose.angle<0) pose.angle+=2*PI;
    while(pose.angle>2*PI) pose.angle-=2*PI;
    return pose;
}

void drawEntity(App* app, Entity* entity){
    Pose pose;
    if(entity->parent == NULL){
        pose = entity->pose;
    }else{
        pose = accumulatePose(entity);
    }
    // Todo: rotation
    blit(app, entity->texture, (int)pose.x, (int)pose.y);
}

void drawScene(App* app)
{
    // RGBA
	SDL_SetRenderDrawColor(app->renderer, 0, 0, 100, 255);
	SDL_RenderClear(app->renderer);

    // Iterate through list
    List* list = app->world->entities;
    while(list!=NULL){
        drawEntity(app, list->data);
        list = list->next;
    }

	SDL_RenderPresent(app->renderer);
}
