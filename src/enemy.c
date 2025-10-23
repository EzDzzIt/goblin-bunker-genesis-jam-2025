#include "level.h"
#include "globals.h"
#include "resources.h"
#include "resources.h"
#include "enemy.h"
#include "player.h"

struct enemyData enemy_array[MAX_ENEMIES];

struct enemyData initEnemy(u8 x, u8 y)
{
    u8 i = 0;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemy_array[i].data.active)
        {
            struct enemyData en;
            en.data.sprite = SPR_addSprite(&demon_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
            en.data.x = x;
            en.data.y = y;
            en.width = 8;
            en.height = 16;
            en.type = ENEMY_TYPE_DEMON;
            en.speed = 1;
            en.hp = 4;
            en.data.active = true;
            SPR_setAnim(en.data.sprite, 0);
            SPR_setAlwaysOnTop(en.data.sprite);
            enemy_array[i] = en;
            break;
        }
    }
}

void killEnemy(u8 index)
{
    enemy_array[index].data.active = false;
    SPR_releaseSprite(enemy_array[index].data.sprite);
    score += 20;
}

void enemyAI(u8 index)

{
    if (global_counter % 3 == 0)
    {
        if (enemy_array[index].type == ENEMY_TYPE_DEMON)
        {
            if (enemy_array[index].data.x >= player.x)
            {
                enemy_array[index].x_velocity = -1 * enemy_array[index].speed;
            }
            else
            {
                enemy_array[index].x_velocity = 1 * enemy_array[index].speed;
            }
            if (enemy_array[index].data.y >= player.y)
            {
                enemy_array[index].y_velocity = -1 * enemy_array[index].speed;
            }
            else
            {
                enemy_array[index].y_velocity = 1 * enemy_array[index].speed;
            }
        }
    }
    else
    {
        enemy_array[index].x_velocity = 0;
        enemy_array[index].y_velocity = 0;
    }
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