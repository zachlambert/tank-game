
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
    char buf[255];
    fscanf(fp, "%s", buf);
    size_t i = 0;
    size_t scanReturn = 0;
    while(scanReturn != EOF){
        scanReturn = fscanf(fp, "%s", buf);
        i++;
    }
    level.width = strlen(buf);
    level.height = i;
    level.data = malloc(level.width*level.height*sizeof(int)); 
    i = 0;
    fclose(fp);
    fp = fopen(filename, "r");
    scanReturn = 0;
    while(scanReturn !=EOF){
        scanReturn = fscanf(fp, "%s", buf);
        memcpy(level.data + i*level.width, buf, level.width);
        i++;
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
