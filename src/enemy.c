#include "level.h"
#include "globals.h"
#include "resources.h"
#include "resources.h"
#include "enemy.h"

struct enemyData enemy_array[5];

struct enemyData initEnemy(u8 x, u8 y)
{

    struct enemyData en;

    en.data.sprite = SPR_addSprite(&demon_sprite, x, y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    en.data.x = x;
    en.data.y = y;

    SPR_setAnim(en.data.sprite, 0);
    return en;
}