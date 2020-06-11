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
    double x,
    double y,
    bool centre,
    Camera* camera)
{
    SDL_Rect dest;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    if(centre){
        x -= dest.w/2;
        y -= dest.h/2;
    }
    dest.x = camera->zoom * (x - camera->x);
    dest.y = camera->zoom * (y - camera->y);
    dest.w *= camera->zoom;
    dest.h *= camera->zoom;
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    // Draw texture onto renderer at rectangle dst.
    // NULL is passed as the src rect to indicate the whole image.
}

void blitRegion(
    SDL_Renderer* renderer,
    SDL_Texture *texture,
    double x,
    double y,
    int index,
    bool centre,
    Camera* camera)
{
    SDL_Rect src = {0, 0, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
    src.w = src.h;
    src.x = src.w * index;
    if(centre){
        x -= src.w/2;
        y -= src.h/2;
    }
    SDL_Rect dest = {
        camera->zoom * (x - camera->x),
        camera->zoom * (y - camera->y),
        camera->zoom * src.w,
        camera->zoom * src.h
    };
    SDL_RenderCopy(renderer, texture, &src, &dest);
}

void drawSprite(
        SDL_Renderer* renderer,
        Pose pose,
        Sprite sprite,
        SpriteData* spriteData,
        bool centre,
        Camera* camera)
{
    if(spriteData->textures[sprite] == NULL) return;
    if(sprite<spriteData->FIRST_ROTATION){
        blit(
            renderer, spriteData->textures[sprite],
            (int)pose.x, (int)pose.y, centre, camera);
    }else{
        int index = (int)round(pose.angle/(2*PI) * spriteData->NUM_ROTATIONS);
        if(index<0) index+= spriteData->NUM_ROTATIONS;
        if(index==spriteData->NUM_ROTATIONS) index = 0;
        blitRegion(
            renderer, spriteData->textures[sprite],
            (int)pose.x, (int)pose.y, index, centre, camera);
    }
}

void drawEntities(
    SDL_Renderer* renderer,
    Entity* entity,
    SpriteData* spriteData,
    Camera* camera)
{
    while(entity){
        drawSprite(
            renderer, entity->data.pose, entity->data.sprite, spriteData, true, camera);
        if(entity->children){
            drawEntities(renderer, entity->children, spriteData, camera);
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
	SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
	SDL_RenderClear(renderer);

    static Pose levelPose = {0, 0, 0};
    drawSprite(renderer, levelPose, world->level->sprite, spriteData, false, &world->camera); 
    drawEntities(renderer, world->entities, spriteData, &world->camera);

	SDL_RenderPresent(renderer);
}
