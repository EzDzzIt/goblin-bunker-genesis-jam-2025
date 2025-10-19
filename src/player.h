#pragma once

#include <genesis.h>

struct playerData
{
    Sprite *sprite;
    fix16 x;
    fix16 y;
};

void initPlayer();
void updatePlayer();