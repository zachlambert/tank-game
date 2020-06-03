#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

typedef struct {
    bool mouse;
    bool w;
    bool s;
    bool a;
    bool d;
} Input;

int doInput(Input* input);

#endif
