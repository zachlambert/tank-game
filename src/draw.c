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
#include "pose.h"

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

void drawSprite(
        SDL_Renderer* renderer,
        Pose pose,
        Sprite sprite,
        SpriteData* spriteData)
{
    if(sprite<spriteData->FIRST_ROTATION){
        blit(
            renderer, spriteData->textures[sprite],
            (int)pose.x, (int)pose.y);
    }else{
        int index = (int)round(pose.angle/(2*PI) * spriteData->NUM_ROTATIONS);
        if(index==spriteData->NUM_ROTATIONS)
            index = 0;
        blitRegion(
            renderer, spriteData->textures[sprite],
            (int)pose.x, (int)pose.y, index);
    }
}

void drawEntities(
    SDL_Renderer* renderer,
    Pose parentPose,
    Entity* entity,
    SpriteData* spriteData)
{
    Pose pose;
    while(entity){
        pose = addPose(parentPose, entity->data.pose);
        drawSprite(renderer, pose, entity->data.sprite, spriteData);
        if(entity->children){
            drawEntities(renderer, pose, entity->children, spriteData);
            // Won't be many layers, recursion probably fine.
            // Can change later if necessary
        }
        entity = entity->next;
    }
}

void drawScene(
    SDL_Renderer* renderer,
    World* world,
    SpriteData* spriteData)
{
    // RGBA
	SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
	SDL_RenderClear(renderer);

    // Use basePose to do camera stuff later
    Pose basePose = { 0, 0, 0 };
    drawEntities(renderer, basePose, world->entities, spriteData);

	SDL_RenderPresent(renderer);
}
