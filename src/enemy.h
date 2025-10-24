#pragma once

#include <genesis.h>
#include "level.h"

// enemy stuff

#define MAX_ENEMIES 10
#define ENEMY_TYPE_DEMON 0
#define ENEMY_TYPE_EYE 1

struct enemyData
{
    struct objectData data;
    u8 width;
    u8 height;
    u8 type;
    f16 x_velocity;
    f16 y_velocity;
    f16 speed;
    s8 hp;
};

extern struct enemyData enemy_array[];

struct enemyData initEnemy(u8 enemy_type, u8 x, u8 y);
void killEnemy(u8 index);
extern void updateEnemies();
extern void enemyAI(u8 index);

// bullet stuff

#define BULLET_WIDTH 8
#define BULLET_HEIGHT 8
#define MAX_BULLETS 16

struct bulletData
{
    struct objectData data;
    Vect2D_f16 velocity;
    u8 lifetime;
};

extern struct bulletData bullet_array[];

void initBullet(u8 x, u8 y, f16 x_velocity, f16 y_velocity);
extern void updateEnemyBullets();