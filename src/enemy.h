#pragma once

#include <genesis.h>
#include "level.h"

// enemy stuff

#define MAX_ENEMIES 12
#define ENEMY_TYPE_DEMON 0
#define ENEMY_TYPE_EYE 1
#define ENEMY_TYPE_SECRET 2
#define ENEMY_TYPE_CULTIST 3

struct enemyData
{
    struct objectData data;
    u8 width;
    u8 height;
    u8 type;
    f16 x_velocity;
    f16 y_velocity;
    f16 speed;
    bool reverse_ai;
    u16 enemy_ai_counter;
    s8 hp;
    s8 hurt_cooldown;
    s8 collided_cooldown;
    bool passthrough;
};

extern struct enemyData enemy_array[];

extern u8 initEnemy(u8 enemy_type, u8 x, u8 y, u8 push_x, u8 push_y, u8 sub_type);
extern void randomEnemySpawn();
extern void randomCultistSpawn();
extern void killEnemy(u8 index);
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