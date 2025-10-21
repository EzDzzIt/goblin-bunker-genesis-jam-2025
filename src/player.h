#pragma once

#include <genesis.h>

#define PLAYER_ANIM_IDLE 0
#define PLAYER_ANIM_TELEPORT 1
#define PLAYER_HEIGHT 2 * 8 // px
#define PLAYER_WIDTH 8      // px

struct playerData
{
    Sprite *sprite;
    fix16 x;
    fix16 y;
    bool hflip;
    fix16 speed;
    u16 last_input;
    Vect2D_f16 velocity;
    u8 jump_cooldown;
    u8 move_cooldown;
};

void initPlayer();
void updatePlayer(u16 time);
void checkInput();
