
#include "level.h"
#include <stdio.h>
#include <string.h>

Level getLevel(char* filename){
    Level level;
    FILE *fp = fopen(filename, "r");
    if(!fp){
        printf("Couldn't read file for level %s\n", filename);
        level.width = 0;
        level.height = 0;
        return level;
    }
    char read;
    int result = 0;
    size_t j, i;
    j = 0;
    result = fscanf(fp, "%c", &read);
    while (result != EOF){
        i = 0;
        while (read != '\n' && result != EOF){
            ++i;
            result = fscanf(fp, "%c", &read);
        }
        result = fscanf(fp, "%c", &read);
        ++j;
    }
    level.width = i;
    level.height = j;
    level.data = malloc(level.width*level.height); 

    fclose(fp);
    fp = fopen(filename, "r");
    j = 0;
    result = fscanf(fp, "%c", &read);
    while(result !=EOF){
        i = 0;
        while (read != '\n' && result != EOF){
            memcpy(level.data + j*level.width + i, &read, 1);
            ++i;
            result = fscanf(fp, "%c", &read);
        }
        result = fscanf(fp, "%c", &read);
        ++j;
    }
    level.sprite = SPRITE_LEVEL;
    fclose(fp);
    return level;
}

Level* initLevels(void){
    Level* levels = calloc(1, sizeof(Level));
    levels[0] = getLevel("levels/level1.txt");
    return levels;
}

char getLevelTile(Level* level, int x, int y){
    if(x>=0 && x<level->width && y>=0 && y<level->height){
        return level->data[level->width*y + x];
    }else{
        return '0';
    }
}
