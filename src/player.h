#pragma once

#include <genesis.h>

#define PLAYER_ANIM_IDLE 0
#define PLAYER_ANIM_WARP 1
#define PLAYER_ANIM_HURT 2
#define PLAYER_ANIM_SPELL 3

#define PLAYER_HEIGHT 2 * 8 // px
#define PLAYER_WIDTH 8      // px

#define PLAYER_HURT_COOLDOWN 50      // frames
#define PLAYER_MOVE_COOLDOWN 5       // frames
#define PLAYER_WARP_COOLDOWN 120     // frames
#define PLAYER_ATTACK_COOLDOWN 35    // frames
#define PLAYER_TELEPORT_CORRECTION 3 // px

struct playerData
{
    Sprite *sprite;
    s16 x;
    s16 y;
    bool hflip;
    fix16 speed;
    u16 last_input;
    Vect2D_f16 velocity;
    s8 warp_cooldown;
    s8 move_cooldown;
    u8 hurt_cooldown;
    s8 attack_cooldown;
    s8 hp;
    u8 cast;
    u8 spell_list;
    u8 tile_x;
    u8 tile_y;
};

extern struct playerData player;

void player_info_print();
void initPlayer();
void updatePlayer();
void checkInput();
