#include "level.h"
#include "globals.h"
#include "resources.h"
#include "enemy.h"

// game logic

void updateGame(u16 time)
{
    u8 i = 0;
    // update doors
    for (i = 0; i < 5; i++)
    {
        if (door_array[i].data.active)
        {
            SPR_setPosition(door_array[i].data.sprite, door_array[i].data.x, door_array[i].data.y);
            // SPR_setHFlip(player.sprite, bullet_array[i].hflip);
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
            SPR_setPosition(enemy_array[i].data.sprite, enemy_array[i].data.x, enemy_array[i].data.y);
        }
    }
    // update bullets
    for (i = 0; i < 5; i++)
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

    // spawn debug

    if (time == 100)
    {
        // SPR_releaseSprite(door_array[4].data.sprite);
        // SPR_releaseSprite(door_array[4].beastmode_sprite);
        door_array[0] = initDoor(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
        initBullet(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 1, 1);
    }
    else if (time == 200)
    {
        door_array[1] = initDoor(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
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
    }
    else if (time == 500)
    {
        // door_array[0] = initDoor(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    }
    else if (time == 600)
    {
        // door_array[1] = initDoor(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    }
}