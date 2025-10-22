#include "level.h"
#include "globals.h"
#include "resources.h"
#include "player.h"

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
            door.beastmode = true;
            door.beastmode_sprite = SPR_addSprite(&doorbeast_sprite, x + 8, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
            door.beastmode_x = x + 8;
            door.beastmode_y = y;
            SPR_setAnim(door.data.sprite, DOOR_OPENING_ANIM);
            door_array[i] = door;
            break;
        }
    }
}

// level initializatin

void initLevel(u8 id)
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