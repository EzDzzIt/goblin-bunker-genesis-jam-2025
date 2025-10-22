#pragma once

#include <genesis.h>

#define PLAYER_ANIM_IDLE 0
#define PLAYER_ANIM_TELEPORT 1
#define PLAYER_ANIM_HURT 2
#define PLAYER_ANIM_SPELL 3

#define PLAYER_HEIGHT 2 * 8 // px
#define PLAYER_WIDTH 8      // px

#define PLAYER_HURT_COOLDOWN 50      // frames
#define PLAYER_MOVE_COOLDOWN 5       // frames
#define PLAYER_WARP_COOLDOWN 120     // frames
#define PLAYER_ATTACK_COOLDOWN 60    // frames
#define PLAYER_TELEPORT_CORRECTION 3 // px

struct playerData
{
    Sprite *sprite;
    fix16 x;
    fix16 y;
    bool hflip;
    fix16 speed;
    u16 last_input;
    Vect2D_f16 velocity;
    s8 warp_cooldown;
    s8 move_cooldown;
    s8 hurt_cooldown;
    s8 attack_cooldown;
    s8 hp;
};

void debug_player_info_print();
void initPlayer();
void updatePlayer();
void checkInput();
