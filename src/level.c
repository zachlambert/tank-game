
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
    printf("%li\n", strlen(buf));
    level.width = 4;
    level.height = 3;
    int levelData[12] = {1,1,1,1,1,0,0,1,1,1,1,1};
    level.data = malloc(level.width*level.height*sizeof(int)); 
    memcpy(level.data, levelData, level.width*level.height*sizeof(int));
    level.sprite = SPRITE_LEVEL;
    return level;
}

Level* initLevels(void){
    Level* levels = calloc(1, sizeof(Level));
    levels[0] = getLevel("levels/level1.txt");
    return levels;
}
