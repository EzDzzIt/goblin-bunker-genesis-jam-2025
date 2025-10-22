#include "level.h"
#include "globals.h"
#include "resources.h"
#include "enemy.h"
#include "player.h"
#include "spell.h"

// game logic

void updateGame(u16 time)
{

    if (time % 60 == 0)
    {
        score += 1;
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
            }
        }
    }
    // update enemies
    for (i = 0; i < 5; i++)
    {
        if (enemy_array[i].data.active)
        {
            // enemy ai
            // enemy_array[i].data.x = player.x - 2;
            // enemy_array[i].data.y = player.y - 2;
            SPR_setPosition(enemy_array[i].data.sprite, enemy_array[i].data.x, enemy_array[i].data.y);
        }
    }
    // update bullets and player projectiles
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
    // for (i = 0; i < MAX_BULLETS; i++)
    // {
    //     if (player_bullet_array[i].data.active)
    //     {
    //         // bullet updates
    //         player_bullet_array[i].data.x += player_bullet_array[i].velocity.x;
    //         player_bullet_array[i].data.y += player_bullet_array[i].velocity.y;
    //         SPR_setPosition(player_bullet_array[i].data.sprite, player_bullet_array[i].data.x, player_bullet_array[i].data.y);
    //         for (i = 0; i < MAX_DOORS; i++)
    //         {
    //             if (door_array[i].data.active)
    //             {
    //                 if (collision_check(player_bullet_array[i].data.x, player_bullet_array[i].data.y, BULLET_WIDTH, BULLET_HEIGHT, door_array[i].data.x, door_array[i].data.y, DOOR_WIDTH, DOOR_HEIGHT))
    //                 {
    //                     // SPR_setPalette(door_array[i].data.sprite, PAL3);
    //                     break;
    //                 }
    //             }
    //         }

    //         if (player_bullet_array[i].data.x >= SCREEN_X_END - BULLET_WIDTH)
    //         {
    //             SPR_releaseSprite(player_bullet_array[i].data.sprite);
    //             player_bullet_array[i].data.active = false;
    //         }
    //         else if (player_bullet_array[i].data.x <= SCREEN_X_OFFSET)
    //         {
    //             SPR_releaseSprite(player_bullet_array[i].data.sprite);
    //             player_bullet_array[i].data.active = false;
    //         }
    //         if (player_bullet_array[i].data.y >= SCREEN_Y_END - BULLET_HEIGHT)
    //         {
    //             SPR_releaseSprite(player_bullet_array[i].data.sprite);
    //             player_bullet_array[i].data.active = false;
    //         }
    //         else if (player_bullet_array[i].data.y <= SCREEN_Y_OFFSET)
    //         {
    //             SPR_releaseSprite(player_bullet_array[i].data.sprite);
    //             player_bullet_array[i].data.active = false;
    //         }
    //     }
    // }

    if (time % 60 == 0)
    {
        // SPR_defragVRAM();
    }

    // spawn debug

    if (time == 100)
    {
        // SPR_releaseSprite(door_array[4].data.sprite);
        // SPR_releaseSprite(door_array[4].beastmode_sprite);
        initDoor(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
        initBullet(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(32 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(16 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(128 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(144 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
    }
    else if (time == 200)
    {
        initDoor(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    }
    else if (time == 300)
    {

        enemy_array[0] = initEnemy(100 + SCREEN_X_OFFSET, 50 + SCREEN_Y_OFFSET);
    }
    else if (time == 400)
    {
        // SPR_releaseSprite(door_array[0].data.sprite);
        // SPR_releaseSprite(door_array[0].beastmode_sprite);
        // SPR_defragVRAM();
        // SPR_reset(); // clears all sprites
        initBullet(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(32 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(16 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(128 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(144 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
    }
    else if (time == 500)
    {
        // door_array[0] = initDoor(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
        initBullet(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(32 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(16 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(128 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(144 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(32 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(16 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(128 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(144 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
    }
    else if (time == 600)
    {
        initBullet(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(32 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(16 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(128 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(144 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(32 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(16 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET, -1, 1);
        initBullet(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET, 1, 1);
        initBullet(128 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, -1);
        initBullet(144 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, -1, 1);
    }
}