#pragma once

#include <genesis.h>

#define PLAYER_ANIM_IDLE 0
#define PLAYER_ANIM_WALK 1

struct playerData
{
    Sprite *sprite;
    fix16 x;
    fix16 y;
    bool hflip;
    fix16 speed;
    u16 last_input;
    Vect2D_f16 velocity;
};

void initPlayer();
void updatePlayer();
void checkInput();
