#include "level.h"
#include "globals.h"
#include "resources.h"
#include "resources.h"
#include "enemy.h"
#include "player.h"

struct enemyData enemy_array[MAX_ENEMIES];

u8 initEnemy(u8 enemy_type, u8 x, u8 y, u8 push_x, u8 push_y)
{
    u8 i = 0;
    u8 return_index = 255;
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
                en.hp = 4;
                en.type = ENEMY_TYPE_DEMON;
            }
            else if (enemy_type == ENEMY_TYPE_EYE)
            {
                en.data.sprite = SPR_addSprite(&eye_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
                en.width = 16;
                en.height = 8;
                en.hp = 2;
                en.type = ENEMY_TYPE_EYE;
            }
            else if (enemy_type == ENEMY_TYPE_SECRET)
            {
                en.data.sprite = SPR_addSprite(&secret_left_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
                en.width = 8;
                en.height = 8;
                en.hp = 3;
                en.type = ENEMY_TYPE_SECRET;
                SPR_setHFlip(en.data.sprite, false);
            }

            en.data.x = x + (push_x - MAP_X) * 8;
            en.data.y = y + (push_y - MAP_Y) * 8;
            en.passthrough = true; // start true so you don't collide with the door
            en.collided_cooldown = 0;
            en.speed = 1;
            en.enemy_ai_counter = 1;
            en.reverse_ai = false;
            en.data.active = true;
            SPR_setAnim(en.data.sprite, 0);
            SPR_setAlwaysOnTop(en.data.sprite);
            enemy_array[i] = en;
            return_index = i;
            break;
        }
    }
    XGM2_playPCM(wav_en_spawn, sizeof(wav_en_spawn), SOUND_PCM_CH_AUTO);
    return return_index;
}

void randomEnemySpawn()
{

    s16 random_x = (random() % (20 - 1 + 1)) + 1;
    s16 random_y = (random() % (16 - 1 + 1)) + 1;

    random_x = random_x * 8 + SCREEN_X_OFFSET;
    random_y = random_y * 8 + SCREEN_Y_OFFSET;

    u8 return_index = 0;

    if ((random() % (100 - 1 + 1)) + 1 <= 20)
    {
        return_index = initEnemy(ENEMY_TYPE_DEMON, random_x, random_y, 0, 0);
        // check to make sure we do not spawn on top of the player!
        if (collision_check(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, enemy_array[return_index].data.x, enemy_array[return_index].data.y, enemy_array[return_index].width, enemy_array[return_index].height))
        {
            enemy_array[return_index].data.x = -1;
            enemy_array[return_index].data.y = -1;
            SPR_setPosition(enemy_array[return_index].data.sprite, -1, -1);
        }
    }
    else
    {
        return_index = initEnemy(ENEMY_TYPE_EYE, random_x, random_y, 0, 0);
        if (collision_check(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, enemy_array[return_index].data.x, enemy_array[return_index].data.y, enemy_array[return_index].width, enemy_array[return_index].height))
        {
            enemy_array[return_index].data.x = -1;
            enemy_array[return_index].data.y = -1;
            SPR_setPosition(enemy_array[return_index].data.sprite, -1, -1);
        }
    }
}

void killEnemy(u8 index)
{
    if (enemy_array[index].type != ENEMY_TYPE_SECRET)
    {
        enemies_killed += 1;
        total_enemies_killed += 1;
        enemy_array[index].data.active = false;
        SPR_releaseSprite(enemy_array[index].data.sprite);
        XGM2_playPCM(wav_en_death, sizeof(wav_en_death), SOUND_PCM_CH_AUTO);
    }
    else
    {
        // secrets_found += 1;
        enemy_array[index].data.active = false;
        SPR_releaseSprite(enemy_array[index].data.sprite);
    }
}

void enemyAI(u8 index)
{

    if (enemy_array[index].type == ENEMY_TYPE_EYE)
    {
        if (enemy_array[index].enemy_ai_counter % 140 == 0)
        {
            if ((random() % (4 - 1 + 1)) + 1 <= 1)
            {
                enemy_array[index].reverse_ai = true;
            }
            else
            {
                enemy_array[index].reverse_ai = false;
            }
            if (enemy_array[index].data.x >= SCREEN_X_END || enemy_array[index].data.x <= SCREEN_X_OFFSET || enemy_array[index].data.y >= SCREEN_Y_END || enemy_array[index].data.y <= SCREEN_Y_OFFSET)
            {
                enemy_array[index].reverse_ai = false;
            }
        }
        // else if (enemy_array[index].reverse_ai || enemy_array[index].enemy_ai_counter % 70 == 0)
        // {
        //     enemy_array[index].reverse_ai = false;
        // }
        else if (enemy_array[index].enemy_ai_counter % 90 == 0)
        {
            if ((random() % (100 - 1 + 1)) + 1 <= level_data.enemy_shot_chance)
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
                // offscreen enemeies can't fire though
                else if (enemy_array[index].data.x >= SCREEN_X_END || enemy_array[index].data.x <= SCREEN_X_OFFSET)
                {
                }
                else if (enemy_array[index].data.y >= SCREEN_Y_END || enemy_array[index].data.y <= SCREEN_Y_OFFSET)
                {
                }
                else
                {
                    XGM2_playPCM(wav_en_shot, sizeof(wav_shot), SOUND_PCM_CH_AUTO);
                    initBullet(enemy_array[index].data.x + 4, enemy_array[index].data.y + 2, bullet_x_velocity, bullet_y_velocity);
                }
            }
        }
        else if (enemy_array[index].enemy_ai_counter % 3 == 0)
        {

            if ((random() % (100 - 1 + 1)) + 1 <= 70)
            {
                s8 flip = 1;
                if (enemy_array[index].reverse_ai)
                {
                    flip = -1;
                }
                if (enemy_array[index].data.x >= player.x)
                {
                    enemy_array[index].x_velocity = -1 * enemy_array[index].speed * flip;
                }
                else
                {
                    enemy_array[index].x_velocity = 1 * enemy_array[index].speed * flip;
                }
                if (enemy_array[index].data.y >= player.y)
                {
                    enemy_array[index].y_velocity = -1 * enemy_array[index].speed * flip;
                }
                else
                {
                    enemy_array[index].y_velocity = 1 * enemy_array[index].speed * flip;
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
        if (enemy_array[index].enemy_ai_counter % 2 == 0)
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
        else if (enemy_array[index].enemy_ai_counter % 240 == 0)
        {
            if ((random() % (2 - 1 + 1)) + 1 <= 1)
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
}

// update enemies
void updateEnemies()
{
    u8 i = 0;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy_array[i].data.active)
        {
            if (enemy_array[i].type == ENEMY_TYPE_SECRET)
            {
                if (UPDATE_SCROLL)
                {
                    enemy_array[i].data.x -= SCROLL_X * 8;
                    enemy_array[i].data.y -= SCROLL_Y * 8;
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
            else
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

                // door collide
                u8 j = 0;
                // collide with doors??? or no?
                if (!enemy_array[i].passthrough)
                {
                    for (j = 0; j < MAX_DOORS; j++)
                    {
                        if (door_array[j].data.active)
                        {
                            if (collision_check(enemy_array[i].data.x, enemy_array[i].data.y, enemy_array[i].width, enemy_array[i].height, door_array[j].data.x, door_array[j].data.y, DOOR_WIDTH, DOOR_HEIGHT))
                            {
                                enemy_array[i].collided_cooldown = 70; // bump cooldown basically
                                enemy_array[i].data.x -= enemy_array[i].x_velocity;
                                enemy_array[i].data.y -= enemy_array[i].y_velocity;
                                break;
                            }
                        }
                    }
                }

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
                enemy_array[i].enemy_ai_counter += 1;
                if (enemy_array[i].enemy_ai_counter > 2000)
                {
                    enemy_array[i].enemy_ai_counter = 1;
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
            if (UPDATE_SCROLL)
            {
                SPR_releaseSprite(bullet_array[i].data.sprite);
                bullet_array[i].data.active = false;
            }
        }
    }
}