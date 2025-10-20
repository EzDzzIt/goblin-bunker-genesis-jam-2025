#pragma once

#include <genesis.h>
#include "level.h"

// enemy stuff

struct enemyData
{
    struct objectData data;
};

extern struct enemyData enemy_array[];

struct enemyData initEnemy(u8 x, u8 y);

// bullet stuff

struct bulletData
{
    struct objectData data;
    Vect2D_f16 velocity;
};

extern struct bulletData bullet_array[];

struct bulletData initBullet(u8 x, u8 y, f16 x_velocity, f16 y_velocity);