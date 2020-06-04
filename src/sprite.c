
#include "sprite.h"
#include <SDL_image.h>

#define PI 3.14159

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

SDL_Texture* loadRotatingTexture(
    SDL_Renderer* renderer, char* filename, const int NUM_ROTATIONS)
{
    // Load the original texture
    SDL_Rect src;
    Uint32 format;
    SDL_Texture* original = loadTexture(renderer, filename);
    if(original == 0)
        return 0;
    // Get the size with SDL_QueryTexture
    SDL_QueryTexture(original, &format, NULL, &src.w, &src.h);

    // Find the bounding square size
    int size = src.w>src.h ? src.w : src.h;
    // Create a dest rect to centre the original on the square
    SDL_Rect dest = { (size-src.w)/2, (size-src.h)/2, src.w, src.h };
    // Create a square rect to write onto
    // It must have the SDL_TEXTUREACCESS_TARGET access 
    SDL_Texture* square = SDL_CreateTexture(
        renderer, format,
        SDL_TEXTUREACCESS_TARGET, size, size);
    // Set render target to this new texture
    SDL_SetRenderTarget(renderer, square);
    // Set TEXTURE blend mode to SDL_BLENDMODE_BLEND to support alpha
    SDL_SetTextureBlendMode(square, SDL_BLENDMODE_BLEND);
    // Set the renderer draw color to be transparent
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // Clear the texture with this colour, setting it to a transparent
    // texture
	SDL_RenderClear(renderer);
    // Copy the original image onto the render target (square)
    SDL_RenderCopy(renderer, original, &src, &dest);

    SDL_Texture* new_texture = SDL_CreateTexture(
        renderer, format, SDL_TEXTUREACCESS_TARGET,
        size*NUM_ROTATIONS, size);

    // Do the same steps as before to clear this texture and
    // set it up for writing to
    SDL_SetRenderTarget(renderer, new_texture);
    SDL_SetTextureBlendMode(new_texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

    dest.x = 0;
    dest.y = 0;
    dest.w = size;
    dest.h = size;
    float angle = 0;

    for(int i=0; i<NUM_ROTATIONS; i++){
        SDL_RenderCopyEx(
            renderer, square, NULL, &dest, angle,
            NULL, SDL_FLIP_NONE);
        dest.x += size;
        angle += (double)360/(double)NUM_ROTATIONS;
    }
    SDL_DestroyTexture(square);
    SDL_DestroyTexture(original);
    SDL_SetRenderTarget(renderer, NULL);
    return new_texture; //new_texture;
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

    const int NUM_ROTATIONS = 64;
    const int FIRST_ROTATION = SPRITE_PLAYER_BASE;

    SDL_Texture* player_base_texture = loadRotatingTexture(
        renderer, "graphics/player_base.png", NUM_ROTATIONS);
    textures[SPRITE_PLAYER_BASE] = player_base_texture;

    SDL_Texture* player_turret_texture = loadRotatingTexture(
        renderer, "graphics/player_turret.png", NUM_ROTATIONS);
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
