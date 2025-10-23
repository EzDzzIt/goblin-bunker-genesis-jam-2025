#pragma once

#include <genesis.h>

#include "player.h"

#define SPELL_CROSS 0
#define PLAYER_BULLET_SPEED_0 2
#define SACRED_GROUND_ANIM 0
#define SACRED_GROUND_WIDTH 16
#define SACRED_GROUND_HEIGHT 16

struct spellData
{
    struct objectData data;
    u8 lifetime;
};

extern struct bulletData player_bullet_array[];
extern struct spellData sacred_ground_array[];

extern void initSpell(u8 spell_type, fix16 x, fix16 y);
extern void initPlayerBullet(fix16 x, fix16 y, f16 x_velocity, f16 y_velocity);
extern void initSacredGround(fix16 x, fix16 y);