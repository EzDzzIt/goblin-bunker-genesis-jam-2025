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