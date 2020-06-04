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
#include "entity.h"
#include "linkedList.h"

#define PI 3.14159

void blit(SDL_Renderer* renderer, SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    dest.x -= dest.w;
    dest.y -= dest.h;
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    // Draw texture onto renderer at rectangle dst.
    // NULL is passed as the src rect to indicate the whole image.
}

void blitRegion(
    SDL_Renderer* renderer, SDL_Texture *texture,
    int x, int y, int index)
{
    SDL_Rect src = {0, 0, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
    src.w = src.h;
    src.x = src.w * index;
    SDL_Rect dest = { 
        x-src.w/2,
        y-src.h/2,
        src.w,
        src.h
    };
    SDL_RenderCopy(renderer, texture, &src, &dest);
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

void drawEntity(SDL_Renderer* renderer, Entity* entity, SpriteData* spriteData)
{
    Pose pose;
    if(entity->parent == NULL){
        pose = entity->pose;
    }else{
        pose = accumulatePose(entity);
    }
    if(entity->sprite<spriteData->FIRST_ROTATION){
        blit(
            renderer, spriteData->textures[entity->sprite],
            (int)pose.x, (int)pose.y);
    }else{
        int index = (int)round(pose.angle/(2*PI) * spriteData->NUM_ROTATIONS);
        if(index==spriteData->NUM_ROTATIONS)
            index = 0;
        blitRegion(
            renderer, spriteData->textures[entity->sprite],
            (int)pose.x, (int)pose.y, index);
    }
}

void drawScene(SDL_Renderer* renderer, World* world, SpriteData* spriteData)
{
    // RGBA
	SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
	SDL_RenderClear(renderer);

    // Iterate through layers
    List** stop = world->layers + LAYER_COUNT;
    List* list;
    for(List** layer = world->layers; layer<stop; layer++){
        list = *layer;
        while(list!=NULL){
            drawEntity(renderer, list->data, spriteData);
            list = list->next;
        }
    }

	SDL_RenderPresent(renderer);
}
