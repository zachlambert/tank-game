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

void blit(
    SDL_Renderer* renderer,
    SDL_Texture *texture,
    int x,
    int y,
    bool centre)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    if(centre){
        dest.x -= dest.w/2;
        dest.y -= dest.h/2;
    }
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    // Draw texture onto renderer at rectangle dst.
    // NULL is passed as the src rect to indicate the whole image.
}

void blitRegion(
    SDL_Renderer* renderer,
    SDL_Texture *texture,
    int x,
    int y,
    int index,
    bool centre)
{
    SDL_Rect src = {0, 0, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
    src.w = src.h;
    src.x = src.w * index;
    SDL_Rect dest = { 
        x,
        y,
        src.w,
        src.h
    };
    if(centre){
        dest.x -= dest.w/2;
        dest.y -= dest.h/2;
    }
    SDL_RenderCopy(renderer, texture, &src, &dest);
}

void drawSprite(
        SDL_Renderer* renderer,
        Pose pose,
        Sprite sprite,
        SpriteData* spriteData,
        bool centre)
{
    if(spriteData->textures[sprite] == NULL) return;
    if(sprite<spriteData->FIRST_ROTATION){
        blit(
            renderer, spriteData->textures[sprite],
            (int)pose.x, (int)pose.y, centre);
    }else{
        int index = (int)round(pose.angle/(2*PI) * spriteData->NUM_ROTATIONS);
        if(index<0) index+= spriteData->NUM_ROTATIONS;
        if(index==spriteData->NUM_ROTATIONS) index = 0;
        blitRegion(
            renderer, spriteData->textures[sprite],
            (int)pose.x, (int)pose.y, index, centre);
    }
}

void drawEntities(
    SDL_Renderer* renderer,
    Entity* entity,
    SpriteData* spriteData)
{
    while(entity){
        drawSprite(
            renderer, entity->data.pose, entity->data.sprite, spriteData, true);
        if(entity->children){
            drawEntities(renderer, entity->children, spriteData);
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

    static Pose levelPose = {0, 0, 0};
    drawSprite(renderer, levelPose, world->level->sprite, spriteData, false); 
    drawEntities(renderer, world->entities, spriteData);

	SDL_RenderPresent(renderer);
}
