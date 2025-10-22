#pragma once

#include <genesis.h>

#include "player.h"

#define SPELL_CROSS 0

extern void initSpell(u8 spell_type, fix16 x, fix16 y);
extern struct bulletData player_bullet_array[];
extern void initPlayerBullet(fix16 x, fix16 y, f16 x_velocity, f16 y_velocity);