
#include "spriteData.h"
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

SDL_Texture* loadLevelTexture(SDL_Renderer* renderer, char* filename, Level* level)
{
    SDL_Rect src;
    Uint32 format;
    SDL_Texture* tile = loadTexture(renderer, filename);
    if(!tile)
        return 0;

    SDL_QueryTexture(tile, &format, NULL, &src.w, &src.h);

    // Set the level tile size
    level->tileWidth = src.w;
    level->tileHeight = src.h;

    // Create texture for entire level
    SDL_Texture* levelTexture = SDL_CreateTexture(
        renderer, format, SDL_TEXTUREACCESS_TARGET,
        level->width*src.w, level->height*src.h);

    SDL_SetRenderTarget(renderer, levelTexture);
    SDL_SetTextureBlendMode(levelTexture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

    size_t index;
    for(size_t j=0; j<level->height; j++){
        for(size_t i=0; i<level->width; i++){
            index = j*level->width + i; 
            SDL_Rect dest = {src.w*i, src.h*j, src.w, src.h};
            if(level->data[index]!='0'){
                SDL_RenderCopy(renderer, tile, NULL, &dest);
            }
        }
    }
    SDL_DestroyTexture(tile);
    SDL_SetRenderTarget(renderer, NULL);
    return levelTexture;
}

SDL_Texture* loadRotatingTexture(
    SDL_Renderer* renderer, char* filename, const int NUM_ROTATIONS)
{
    // Load the original texture
    SDL_Rect src = {0, 0, 0, 0};
    Uint32 format;
    int access;
    SDL_Texture* original = loadTexture(renderer, filename);
    if(original == 0)
        return 0;
    // Get the size with SDL_QueryTexture
    SDL_QueryTexture(original, &format, &access, &src.w, &src.h);

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
    SDL_RenderCopy(renderer, original, NULL, &dest);

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

SpriteData* initSpriteData(SDL_Renderer* renderer, Level* levels)
{
    int count = SPRITE_COUNT;
    SDL_Texture** textures = calloc(count, sizeof(SDL_Texture*));

    // Non-rotated sprites

    SDL_Texture* levelTexture = loadLevelTexture(
        renderer, "graphics/levelTile.png", levels);
    textures[SPRITE_LEVEL] = levelTexture;

    // Rotated-sprites

    const int NUM_ROTATIONS = 64;
    const int FIRST_ROTATION = SPRITE_TANK_BLUE_BASE;

    SDL_Texture* tankBlueBaseTexture = loadRotatingTexture(
        renderer, "graphics/tankBlueBase.png", NUM_ROTATIONS);
    textures[SPRITE_TANK_BLUE_BASE] = tankBlueBaseTexture;

    SDL_Texture* tankBlueTurretTexture = loadRotatingTexture(
        renderer, "graphics/tankBlueTurret.png", NUM_ROTATIONS);
    textures[SPRITE_TANK_BLUE_TURRET] = tankBlueTurretTexture;
    
    SDL_Texture* tankRedBaseTexture = loadRotatingTexture(
        renderer, "graphics/tankRedBase.png", NUM_ROTATIONS);
    textures[SPRITE_TANK_RED_BASE] = tankRedBaseTexture;

    SDL_Texture* tankRedTurretTexture = loadRotatingTexture(
        renderer, "graphics/tankRedTurret.png", NUM_ROTATIONS);
    textures[SPRITE_TANK_RED_TURRET] = tankRedTurretTexture;



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
