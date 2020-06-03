
#include "sprite.h"
#include <SDL_image.h>

SDL_Texture* loadTexture(SDL_Renderer* renderer, char* filename)
{
    SDL_Texture* texture;
    SDL_LogMessage(
        SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
        "Loading %s", filename);
    texture = IMG_LoadTexture(renderer, filename);
    if(texture == NULL)
    {
        SDL_LogMessage(
            SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
            "Failed to load %s", filename);
    }
    return texture;
}

SDL_Texture* loadPreRotatedTexture(SDL_Renderer* renderer, char* filename)
{
    //todo
    return 0;
}

SpriteData* initSpriteData(SDL_Renderer* renderer)
{
    int count = SPRITE_COUNT;
    SDL_Texture** textures = calloc(count, sizeof(SDL_Texture*));

    // Create a texture for each sprite, otherwise it can just be left
    // as a null pointer as a placeholder
    // Don't need to initialise in the order they appear in the enum
    // but it is useful and makes sure FIRST_ROTATION is correct.

    // Non-rotated sprites - Used as is

    // Rotated-sprites - All sprites in this section pre-render
    // sprites for each rotation, and when drawing, index by
    // the quantised rotation.

    const int NUM_ROTATIONS = 16;
    const int FIRST_ROTATION = SPRITE_PLAYER_BASE;

    SDL_Texture* player_base_texture = loadTexture(
        renderer, "graphics/player_base.png");
    textures[SPRITE_PLAYER_BASE] = player_base_texture;

    SDL_Texture* player_turret_texture = loadTexture(
        renderer, "graphics/player_turret.png");
    textures[SPRITE_PLAYER_TURRET] = player_turret_texture;

    // In order to assign memory to a struct with const elements,
    // you need to initialise the object with an initialiser list.
    // However, this means that it is a statically allocated object, so
    // is on the stack, and can't be accessed outside the function.
    // To dynamically allocate an object, you need to allocate memory
    // for it, then copy this statically allocated object, then return
    // the address of the dynamically allocated object.
    SpriteData spriteDataInit = {
        .FIRST_ROTATION = FIRST_ROTATION,
        .NUM_ROTATIONS = NUM_ROTATIONS,
        .textures = textures
    };
    SpriteData* spriteData = malloc(sizeof(SpriteData));
    memcpy(spriteData, &spriteDataInit, sizeof(SpriteData));

    return spriteData;
}
