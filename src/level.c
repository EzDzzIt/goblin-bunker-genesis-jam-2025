#include "level.h"
#include "globals.h"
#include "resources.h"
#include "resources.h"
#include "enemy.h"

// level objects

struct doorData door_array[5];

struct doorData initDoor(u8 x, u8 y)
{

    struct doorData door;

    door.data.sprite = SPR_addSprite(&door_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    door.data.x = x;
    door.data.y = y;
    door.beastmode = true;
    door.beastmode_sprite = SPR_addSprite(&doorbeast_sprite, x + 8, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    door.beastmode_x = x + 8;
    door.beastmode_y = y;

    SPR_setAnim(door.data.sprite, DOOR_OPENING_ANIM);
    return door;
}

// level logic

void updateLevel(u16 time)
{
    u8 i = 0;
    for (i = 0; i < 5; i++)
    {
        if (door_array[i].data.active)
        {
            SPR_setPosition(door_array[i].data.sprite, door_array[i].data.x, door_array[i].data.y);
            // SPR_setHFlip(player.sprite, player.hflip);
            if (door_array[i].beastmode)
            {
                SPR_setPosition(door_array[i].beastmode_sprite, door_array[i].beastmode_x, door_array[i].beastmode_y);
            }
        }
    }
    for (i = 0; i < 5; i++)
    {
        if (enemy_array[i].data.active)
        {
            SPR_setPosition(enemy_array[i].data.sprite, enemy_array[i].data.x, enemy_array[i].data.y);
        }
    }
    if (time == 100)
    {
        // SPR_releaseSprite(door_array[4].data.sprite);
        // SPR_releaseSprite(door_array[4].beastmode_sprite);
        door_array[0] = initDoor(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    }
    else if (time == 200)
    {
        door_array[1] = initDoor(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    }
    else if (time == 300)
    {
        SPR_releaseSprite(door_array[1].data.sprite);
        SPR_releaseSprite(door_array[1].beastmode_sprite);

        enemy_array[1] = initEnemy(100 + SCREEN_X_OFFSET, 50 + SCREEN_Y_OFFSET);
    }
    else if (time == 400)
    {
        SPR_releaseSprite(door_array[0].data.sprite);
        SPR_releaseSprite(door_array[0].beastmode_sprite);
        SPR_defragVRAM();
        // SPR_reset(); // clears all sprites
    }
    else if (time == 500)
    {
        door_array[0] = initDoor(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    }
    else if (time == 600)
    {
        door_array[1] = initDoor(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    }
}

void initLevel(u8 id)
{
    // start by cleaning up old level memory
    // load in new level data based on the level id parameter
    // door_array[0] = initDoor(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    // door_array[1] = initDoor(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    // door_array[2] = initDoor(128 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
    // door_array[3] = initDoor(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET);
    // door_array[4] = initDoor(16 + SCREEN_X_OFFSET, 128 + SCREEN_Y_OFFSET);
    VDP_loadTileSet(&level_tileset, 0, DMA);
    VDP_setTileMapEx(BG_B, &level_map, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 0, 0, 0, 0, 32, 28, DMA);
}