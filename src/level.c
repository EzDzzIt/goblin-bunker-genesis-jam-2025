#include "level.h"
#include "globals.h"
#include "resources.h"

// level objects

struct doorData door_array[5];

struct doorData initDoor(u8 x, u8 y)
{

    struct doorData door;

    door.data.sprite = SPR_addSprite(&door_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    door.data.x = x;
    door.data.y = y;
    door.data.active = true;
    door.beastmode = true;
    door.beastmode_sprite = SPR_addSprite(&doorbeast_sprite, x + 8, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    door.beastmode_x = x + 8;
    door.beastmode_y = y;

    SPR_setAnim(door.data.sprite, DOOR_OPENING_ANIM);
    return door;
}

// level initializatin

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
    VDP_setTileMapEx(BG_A, &level_map, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 0, 0, 0, 0, 32, 28, DMA);
    // this index needs to be adjusted based on the size in tiles of the main level tileset
    VDP_loadTileSet(&border_tileset, 20, DMA);
    VDP_setTileMapEx(BG_B, &border_image, TILE_ATTR_FULL(PAL3, 0, FALSE, FALSE, 20), 0, 0, 0, 0, 32, 28, DMA);
}