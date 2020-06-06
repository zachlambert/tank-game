#ifndef LEVEL_H
#define LEVEL_H

#include "sprite.h"

typedef struct{
    int width;
    int height;
    int* data;
    Sprite sprite;
}Level;

Level getLevel(char* filename);
Level* initLevels(void);

#endif
