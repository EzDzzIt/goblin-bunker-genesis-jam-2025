#include "level.h"
#include "globals.h"
#include "resources.h"
#include "resources.h"
#include "enemy.h"

struct enemyData enemy_array[MAX_ENEMIES];

struct enemyData initEnemy(u8 x, u8 y)
{

    struct enemyData en;

    en.data.sprite = SPR_addSprite(&demon_sprite, x, y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    en.data.x = x;
    en.data.y = y;
    en.width = 8;
    en.height = 16;
    en.data.active = true;

    SPR_setAnim(en.data.sprite, 0);

    return en;
}

void deinitEnemy(u8 i)
{
    enemy_array[i].data.active = false;
    // SPR_releaseSprite(enemy_array[i].data.sprite);
}

// bullet stuff

struct bulletData bullet_array[MAX_BULLETS];

void initBullet(u8 x, u8 y, f16 x_velocity, f16 y_velocity)
{

    u8 i = 0;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullet_array[i].data.active)
        {
            struct bulletData bul;
            bul.data.sprite = SPR_addSprite(&bullet_sprite, x, y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
            bul.data.x = x;
            bul.data.y = y;
            bul.data.active = true;
            bul.velocity.x = x_velocity;
            bul.velocity.y = y_velocity;
            SPR_setAnim(bul.data.sprite, 0);
            bullet_array[i] = bul;
            break;
        }
    }
}