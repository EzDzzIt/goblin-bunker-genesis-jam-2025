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

void updateDoors()
{
    u8 i = 0;
    for (i = 0; i < MAX_DOORS; i++)
    {
        if (door_array[i].data.active)
        {
            if (UPDATE_SCROLL)
            {
                // door_array[i].data.x -= SCROLL_X * 8;
                // door_array[i].data.y -= SCROLL_Y * 8;
                if (door_array[i].data.y > SCREEN_Y_END || door_array[i].data.y < SCREEN_Y_OFFSET)
                {
                    // SPR_setVisibility(door_array[i].data.sprite, FALSE);
                }
                else
                {
                    // SPR_setVisibility(door_array[i].data.sprite, TRUE);
                }
            }
            SPR_setPosition(door_array[i].data.sprite, door_array[i].data.x, door_array[i].data.y);
            if (door_array[i].beastmode)
            {
                SPR_setPosition(door_array[i].beastmode_sprite, door_array[i].beastmode_x, door_array[i].beastmode_y);
                door_array[i].beastmode_counter += 1;
                if (door_array[i].beastmode_counter >= levelObject.beastmode_time_limit)
                {
                    toggleDoorBeastmode(i);
                    SPR_setAnim(door_array[i].data.sprite, 2);

                    if ((random() % (100 - 1 + 1)) + 1 <= 25)
                    {
                        initEnemy(ENEMY_TYPE_EYE, door_array[i].data.x, door_array[i].data.y);
                    }
                    else
                    {
                        initEnemy(ENEMY_TYPE_DEMON, door_array[i].data.x, door_array[i].data.y);
                    }
                    // door_array[i].data.active = false;
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
    VDP_setTileMapEx(BG_B, &level_map, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 6, 5, SCROLL_X, SCROLL_Y, 20, 16, DMA);
    // VDP_setTileMapEx(BG_A, &window_ui, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 6, 21, 0, 0, 20, 2, DMA);
    // this index needs to be adjusted based on the size in tiles of the main level tileset
    VDP_loadTileSet(&border_tileset, 20, DMA);
    VDP_setTileMapEx(BG_A, &border_image, TILE_ATTR_FULL(PAL3, 1, FALSE, FALSE, 20), 0, 0, 0, 0, 32, 28, DMA);
}

void updateLevel(u8 level)
{
    if (level == 0)
    {
        if (global_counter == 2) // 2 is the earliest global counter for spawning stuff
        {
            levelObject.map_height = 16 * 3 - 16; // 16 tiles per screen height
            levelObject.map_width = 20 * 2 - 20;  // 20 tiles per screen width
            levelObject.beastmode_chance = 100;
            levelObject.beastmode_time_limit = 200;
            levelObject.enemy_shot_chance = 60; // percent
            initDoor(16 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
            initDoor(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
            initDoor(16 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET);
            initDoor(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET);
            initEnemy(ENEMY_TYPE_DEMON, 50, 50);
        }
        else if (global_counter == 100)
        {
            // levelObject.beastmode_chance = 10;
            // levelObject.beastmode_time_limit = 200;
            // initDoor(96 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET);
            // initDoor(96 + SCREEN_X_OFFSET, 96 + SCREEN_Y_OFFSET);
            // initDoor(96 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET);
            // initDoor(96 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET);
        }
    }
    if (UPDATE_SCROLL)
    {
        UPDATE_SCROLL = false;
        MAP_X += SCROLL_X;
        MAP_Y += SCROLL_Y;
        SCROLL_X = 0;
        SCROLL_Y = 0;
        VDP_setTileMapEx(BG_B, &level_map, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 6, 5, MAP_X, MAP_Y, 20, 16, DMA);
    }
}