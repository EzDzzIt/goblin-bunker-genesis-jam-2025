#include "level.h"
#include "globals.h"
#include "resources.h"
#include "player.h"
#include "enemy.h"

// level objects

struct doorData door_array[MAX_DOORS];

void initDoor(u8 x, u8 y)
{

    u8 i = 0;
    for (i = 0; i < MAX_DOORS; i++)
    {
        if (!door_array[i].data.active)
        {
            struct doorData door;
            door.data.sprite = SPR_addSprite(&door_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
            door.data.x = x;
            door.data.y = y;
            door.data.active = true;
            door.beastmode = false;
            // door.beastmode_sprite = SPR_addSprite(&doorbeast_sprite, x + 8, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
            // door.beastmode_x = x + 8;
            // door.beastmode_y = y;
            // SPR_setAnim(door.data.sprite, DOOR_OPENING_ANIM);
            door_array[i] = door;
            break;
        }
    }
}

void toggleDoorBeastmode(u8 index)
{
    if (!door_array[index].beastmode)
    {
        door_array[index].beastmode = true;
        door_array[index].beastmode_sprite = SPR_addSprite(&doorbeast_sprite, door_array[index].data.x + 8, door_array[index].data.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
        door_array[index].beastmode_x = door_array[index].data.x + 8;
        door_array[index].beastmode_y = door_array[index].data.y;
        SPR_setAnim(door_array[index].data.sprite, DOOR_OPENING_ANIM);
    }
    else
    {
        door_array[index].beastmode = false;
        SPR_releaseSprite(door_array[index].beastmode_sprite);
        SPR_setAnim(door_array[index].data.sprite, 0); // reset to static door
        door_array[index].beastmode_counter = 0;
    }
}

// level initializatin

struct levelData levelObject;

void initLevel(u8 level)
{
    clear_graphics(FALSE);

    XGM2_setFMVolume(40); // debug
    XGM2_play(xgm2_level0);
    XGM2_fadeIn(150);

    initPlayer();

    // load in new level data based on the level id parameter
    VDP_loadTileSet(&level_tileset, 0, DMA);
    VDP_setTileMapEx(BG_A, &level_map, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 6, 5, 0, 0, 20, 18, DMA);
    // this index needs to be adjusted based on the size in tiles of the main level tileset
    VDP_loadTileSet(&border_tileset, 20, DMA);
    VDP_setTileMapEx(BG_B, &border_image, TILE_ATTR_FULL(PAL3, 0, FALSE, FALSE, 20), 0, 0, 0, 0, 32, 28, DMA);
}

void updateLevel(u8 level)
{
    if (level == 0)
    {
        if (global_counter == 2) // 2 is the earliest global counter for spawning stuff
        {
            levelObject.beastmode_chance = 1;
            levelObject.beastmode_time_limit = 300;
            initDoor(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
            initDoor(32 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
            initDoor(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
            initDoor(16 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET);
            initDoor(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET);
        }
    }
}