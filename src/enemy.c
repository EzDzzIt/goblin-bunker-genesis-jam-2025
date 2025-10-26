#include "level.h"
#include "globals.h"
#include "resources.h"
#include "resources.h"
#include "enemy.h"
#include "player.h"

struct enemyData enemy_array[MAX_ENEMIES];

struct enemyData initEnemy(u8 enemy_type, u8 x, u8 y)
{
    u8 i = 0;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemy_array[i].data.active)
        {
            struct enemyData en;
            if (enemy_type == ENEMY_TYPE_DEMON)
            {
                en.data.sprite = SPR_addSprite(&demon_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
                en.width = 8;
                en.height = 16;
                en.type = ENEMY_TYPE_DEMON;
            }
            else if (enemy_type == ENEMY_TYPE_EYE)
            {
                en.data.sprite = SPR_addSprite(&eye_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
                en.width = 16;
                en.height = 8;
                en.type = ENEMY_TYPE_EYE;
            }
            en.data.x = x;
            en.data.y = y;

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

    if (enemy_array[index].type == ENEMY_TYPE_EYE)
    {
        if (global_counter % 120 == 0)
        {
            if ((random() % (100 - 1 + 1)) + 1 <= levelObject.enemy_shot_chance)
            {
                s8 bullet_x_velocity = 0;
                s8 bullet_y_velocity = 0;
                if (enemy_array[index].data.x >= player.x + PLAYER_WIDTH)
                {
                    bullet_x_velocity = -1;
                }
                else if (enemy_array[index].data.x <= player.x - PLAYER_WIDTH)
                {
                    bullet_x_velocity = 1;
                }
                if (enemy_array[index].data.y >= player.y + PLAYER_HEIGHT)
                {
                    bullet_y_velocity = -1;
                }
                else if (enemy_array[index].data.y <= player.y - PLAYER_HEIGHT)
                {
                    bullet_y_velocity = 1;
                }
                if (bullet_x_velocity == 0 && bullet_y_velocity == 0)
                {
                }
                else
                {
                    initBullet(enemy_array[index].data.x, enemy_array[index].data.y, bullet_x_velocity, bullet_y_velocity);
                }
            }
        }
        else if (global_counter % 3 == 0)
        {
            if ((random() % (100 - 1 + 1)) + 1 <= 70)
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
            else
            {
                enemy_array[index].x_velocity = 0;
                enemy_array[index].y_velocity = 0;
            }
        }

        else
        {
            enemy_array[index].x_velocity = 0;
            enemy_array[index].y_velocity = 0;
        }
    }
    else if (enemy_array[index].type == ENEMY_TYPE_DEMON)
    {
        if (global_counter % 2 == 0)
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

        else
        {
            enemy_array[index].x_velocity = 0;
            enemy_array[index].y_velocity = 0;
        }
    }
}

// update enemies
void updateEnemies()
{
    u8 i = 0;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy_array[i].data.active)
        {
            enemyAI(i); // ai calc to get x/y velocity
            // apply velocity

            if (UPDATE_SCROLL)
            {
                enemy_array[i].data.x -= SCROLL_X * 8;
                enemy_array[i].data.y -= SCROLL_Y * 8;
            }
            enemy_array[i].data.x += enemy_array[i].x_velocity;
            enemy_array[i].data.y += enemy_array[i].y_velocity;

            if (enemy_array[i].x_velocity < 0)
            {
                SPR_setHFlip(enemy_array[i].data.sprite, FALSE);
            }
            else
            {
                SPR_setHFlip(enemy_array[i].data.sprite, TRUE);
            }
            SPR_setPosition(enemy_array[i].data.sprite, enemy_array[i].data.x, enemy_array[i].data.y);
            // setSprite(enemy_array[i].data.sprite, enemy_array[i].data.x, enemy_array[i].data.y);
            if (enemy_array[i].hp <= 0)
            {
                killEnemy(i);
            }
            if (enemy_array[i].hurt_cooldown > 0)
            {
                enemy_array[i].hurt_cooldown -= 1;
                if (enemy_array[i].hurt_cooldown % 2 == 0)
                {
                    SPR_setPalette(enemy_array[i].data.sprite, PAL1);
                }
                else
                {
                    SPR_setPalette(enemy_array[i].data.sprite, PAL3);
                }
            }
        }
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
            bul.lifetime = 120;
            SPR_setAnim(bul.data.sprite, 0);
            bullet_array[i] = bul;
            break;
        }
    }
}

void updateEnemyBullets()
{
    u8 i = 0;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet_array[i].data.active)
        {
            // bullet updates
            bullet_array[i].data.x += bullet_array[i].velocity.x;
            bullet_array[i].data.y += bullet_array[i].velocity.y;
            SPR_setPosition(bullet_array[i].data.sprite, bullet_array[i].data.x, bullet_array[i].data.y);
            if (bullet_array[i].data.x >= SCREEN_X_END - BULLET_WIDTH)
            {
                SPR_releaseSprite(bullet_array[i].data.sprite);
                bullet_array[i].data.active = false;
            }
            else if (bullet_array[i].data.x <= SCREEN_X_OFFSET)
            {
                SPR_releaseSprite(bullet_array[i].data.sprite);
                bullet_array[i].data.active = false;
            }
            if (bullet_array[i].data.y >= SCREEN_Y_END - BULLET_HEIGHT)
            {
                SPR_releaseSprite(bullet_array[i].data.sprite);
                bullet_array[i].data.active = false;
            }
            else if (bullet_array[i].data.y <= SCREEN_Y_OFFSET)
            {
                SPR_releaseSprite(bullet_array[i].data.sprite);
                bullet_array[i].data.active = false;
            }
            bullet_array[i].lifetime -= 1;
            if (bullet_array[i].lifetime <= 0)
            {
                SPR_releaseSprite(bullet_array[i].data.sprite);
                bullet_array[i].data.active = false;
            }
        }
    }
}