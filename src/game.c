#include "level.h"
#include "globals.h"
#include "resources.h"
#include "enemy.h"
#include "player.h"
#include "spell.h"

// game logic

void updateGame()
{

    if (global_counter % 120 == 0)
    {
        // score += 1;
        SPR_defragVRAM();
    }

    u8 i = 0;
    // update doors
    for (i = 0; i < MAX_DOORS; i++)
    {
        if (door_array[i].data.active)
        {
            SPR_setPosition(door_array[i].data.sprite, door_array[i].data.x, door_array[i].data.y);
            if (door_array[i].beastmode)
            {
                SPR_setPosition(door_array[i].beastmode_sprite, door_array[i].beastmode_x, door_array[i].beastmode_y);
                door_array[i].beastmode_counter += 1;
                if (door_array[i].beastmode_counter >= levelObject.beastmode_time_limit)
                {
                    toggleDoorBeastmode(i);
                    SPR_setAnim(door_array[i].data.sprite, 2);
                    initEnemy(door_array[i].data.x, door_array[i].data.y);
                    door_array[i].data.active = false;
                }
            }
            else
            {
                if ((random() % (1000 - 1 + 1)) + 1 <= levelObject.beastmode_chance)
                {
                    toggleDoorBeastmode(i);
                }
            }
        }
    }
    // update enemies
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy_array[i].data.active)
        {
            enemyAI(i); // ai calc to get x/y velocity
            // apply velocity
            enemy_array[i].data.x += enemy_array[i].x_velocity;
            enemy_array[i].data.y += enemy_array[i].y_velocity;
            SPR_setPosition(enemy_array[i].data.sprite, enemy_array[i].data.x, enemy_array[i].data.y);
            if (enemy_array[i].hp <= 0)
            {
                killEnemy(i);
            }
        }
    }
    // update enemy bullets
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
        }
    }
    // update player spells and projectiles
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (player_bullet_array[i].data.active)
        {
            // bullet updates
            player_bullet_array[i].data.x += player_bullet_array[i].velocity.x;
            player_bullet_array[i].data.y += player_bullet_array[i].velocity.y;
            SPR_setPosition(player_bullet_array[i].data.sprite, player_bullet_array[i].data.x, player_bullet_array[i].data.y);
            bool collided = false;
            u8 j = 0;
            // for (j = 0; j < MAX_DOORS; j++)
            // {
            //     if (door_array[j].data.active)
            //     {
            //         if (collision_check(player_bullet_array[i].data.x, player_bullet_array[i].data.y, BULLET_WIDTH, BULLET_HEIGHT, door_array[j].data.x, door_array[j].data.y, DOOR_WIDTH, DOOR_HEIGHT))
            //         {
            //             SPR_setPalette(door_array[j].data.sprite, PAL3);
            //             collided = true;
            //             break;
            //         }
            //     }
            // }
            for (j = 0; j < MAX_ENEMIES; j++)
            {
                if (enemy_array[j].data.active)
                {
                    if (collision_check(player_bullet_array[i].data.x, player_bullet_array[i].data.y, BULLET_WIDTH, BULLET_HEIGHT, enemy_array[j].data.x, enemy_array[j].data.y, enemy_array[j].width, enemy_array[j].height))
                    {
                        SPR_setPalette(enemy_array[j].data.sprite, PAL3);
                        enemy_array[j].hp -= 1;
                        collided = true;
                        break;
                    }
                }
            }
            if (collided)
            {
                SPR_releaseSprite(player_bullet_array[i].data.sprite);
                player_bullet_array[i].data.active = false;
            }
            else
            {
                if (player_bullet_array[i].data.x >= SCREEN_X_END - BULLET_WIDTH)
                {
                    SPR_releaseSprite(player_bullet_array[i].data.sprite);
                    player_bullet_array[i].data.active = false;
                }
                else if (player_bullet_array[i].data.x <= SCREEN_X_OFFSET)
                {
                    SPR_releaseSprite(player_bullet_array[i].data.sprite);
                    player_bullet_array[i].data.active = false;
                }
                if (player_bullet_array[i].data.y >= SCREEN_Y_END - BULLET_HEIGHT)
                {
                    SPR_releaseSprite(player_bullet_array[i].data.sprite);
                    player_bullet_array[i].data.active = false;
                }
                else if (player_bullet_array[i].data.y <= SCREEN_Y_OFFSET)
                {
                    SPR_releaseSprite(player_bullet_array[i].data.sprite);
                    player_bullet_array[i].data.active = false;
                }
            }
        }
    }
    // update sacred ground
    for (i = 0; i < 2; i++)
    {
        if (sacred_ground_array[i].lifetime > 0)
        {
            sacred_ground_array[i].lifetime -= 1;
            if (sacred_ground_array[i].lifetime == 0)
            {
                sacred_ground_array[i].data.active = false;
                SPR_releaseSprite(sacred_ground_array[i].data.sprite);
                continue;
            }
            if (sacred_ground_array[i].data.active)
            {
                u8 j = 0;
                for (j = 0; j < MAX_DOORS; j++)
                {
                    // collision check between sacred ground and doors needs slight mod to work
                    if (door_array[j].beastmode)
                    {
                        if (collision_check(sacred_ground_array[i].data.x - 2, sacred_ground_array[i].data.y - 3, SACRED_GROUND_WIDTH + 2, SACRED_GROUND_HEIGHT + 3, door_array[j].data.x, door_array[j].data.y, DOOR_WIDTH, DOOR_HEIGHT))
                        {
                            toggleDoorBeastmode(j);
                            score += 10;
                        }
                    }
                }
            }
        }
    }
}
