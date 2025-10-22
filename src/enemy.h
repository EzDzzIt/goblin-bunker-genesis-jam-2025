#pragma once

#include <genesis.h>
#include "level.h"

// enemy stuff

struct enemyData
{
    struct objectData data;
    u8 width;
    u8 height;
};

extern struct enemyData enemy_array[];

struct enemyData initEnemy(u8 x, u8 y);
void deinitEnemy(u8 i);

// bullet stuff

#define BULLET_WIDTH 8
#define BULLET_HEIGHT 8
#define MAX_BULLETS 16

struct bulletData
{
    struct objectData data;
    Vect2D_f16 velocity;
};

extern struct bulletData bullet_array[];

void initBullet(u8 x, u8 y, f16 x_velocity, f16 y_velocity);