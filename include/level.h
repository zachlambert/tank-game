#ifndef LEVEL_H
#define LEVEL_H

#include "sprite.h"

typedef struct{
    size_t width;
    size_t height;
    char* data;
    Sprite sprite;
    size_t tileWidth;
    size_t tileHeight;
}Level;

Level getLevel(char* filename);
Level* initLevels(void);

#endif
