#pragma once

#include <genesis.h>

struct playerData
{
    Sprite *sprite;
    fix16 x;
    fix16 y;
    Vect2D_s16 input;
};

void initPlayer();
void updatePlayer();
void checkInput();