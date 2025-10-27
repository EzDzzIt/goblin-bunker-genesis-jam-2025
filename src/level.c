#include "level.h"
#include "globals.h"
#include "resources.h"
#include "player.h"
#include "enemy.h"

// level objects

struct doorData door_array[MAX_DOORS];

void initDoor(u8 x, u8 y, u8 push_x, u8 push_y)
{

    u8 i = 0;
    for (i = 0; i < MAX_DOORS; i++)
    {
        if (!door_array[i].data.active)
        {
            struct doorData door;
            door.data.x = x;
            door.data.y = y;
            door.data.sprite = SPR_addSprite(&door_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
            door.data.active = true;
            door.beastmode = false;
            door.shut_count = 0;
            door.sealed = false;
            door.close_cooldown = 0;
            door.push_x = push_x;
            door.push_y = push_y;
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
                door_array[i].data.x -= SCROLL_X * 8;
                door_array[i].data.y -= SCROLL_Y * 8;
            }
            SPR_setPosition(door_array[i].data.sprite, door_array[i].data.x, door_array[i].data.y);
            if (door_array[i].beastmode)
            {
                if (UPDATE_SCROLL)
                {
                    door_array[i].beastmode_x -= SCROLL_X * 8;
                    door_array[i].beastmode_y -= SCROLL_Y * 8;
                }

                SPR_setPosition(door_array[i].beastmode_sprite, door_array[i].beastmode_x, door_array[i].beastmode_y);
                door_array[i].beastmode_counter += 1;
                if (door_array[i].beastmode_counter >= levelObject.beastmode_time_limit)
                {
                    // toggleDoorBeastmode(i);
                    door_array[i].beastmode_counter = 0;

                    if ((random() % (100 - 1 + 1)) + 1 <= 25)
                    {
                        initEnemy(ENEMY_TYPE_EYE, door_array[i].data.x, door_array[i].data.y);
                    }
                    else
                    {
                        initEnemy(ENEMY_TYPE_DEMON, door_array[i].data.x, door_array[i].data.y);
                    }
                }
            }
            else
            {
                if (!door_array[i].sealed)
                {
                    if ((random() % (1000 - 1 + 1)) + 1 <= levelObject.beastmode_chance)
                    {
                        if (door_array[i].close_cooldown <= 0)
                        {
                            toggleDoorBeastmode(i);
                        }
                    }
                }
            }
            if (door_array[i].close_cooldown > 0)
            {
                door_array[i].close_cooldown -= 1;
            }
        }
    }
}

void shutDoor(u8 index)
{
    toggleDoorBeastmode(index);
    score += 10;
    if (door_array[index].shut_count >= levelObject.shuts_to_seal)
    {
        // seal door
        door_array[index].sealed = true;
        SPR_setAnim(door_array[index].data.sprite, DOOR_SEALED_ANIM);
        doors_closed += 1;
        score += 50;
    }
    else
    {

        door_array[index].shut_count += 1;
        door_array[index].close_cooldown = 60;
    }
}

void applyDoorOffsets()
{
    u8 i = 0;
    for (i = 0; i < MAX_DOORS; i++)
    {
        if (door_array[i].data.active)
        {
            door_array[i].data.x += door_array[i].push_x * 8;
            door_array[i].data.y += door_array[i].push_y * 8;
        }
    }
}

// level initializatin

struct levelData levelObject;

void initLevel(u8 level)
{
    // clear_graphics(FALSE);

    XGM2_setFMVolume(40); // debug
    XGM2_play(xgm2_level0);
    XGM2_fadeIn(150);

    current_level = level;
    reset_globals();
    SPR_defragVRAM();

    if (level == 1)
    {
        currentMap = &level_1_map;
    }
    else if (level == 2)
    {
        currentMap = &level_2_map;
    }
    else if (level == 3)
    {
        currentMap = &level_3_map;
    }
    VDP_loadTileSet(&level_tileset, 0, DMA);
    VDP_setTileMapEx(BG_B, currentMap, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 6, 5, SCROLL_X, SCROLL_Y, 20, 16, DMA);
    VDP_loadTileSet(&border_tileset, level_tileset.numTile, DMA);
    VDP_setTileMapEx(BG_A, &border_image, TILE_ATTR_FULL(PAL3, 1, FALSE, FALSE, level_tileset.numTile), 0, 0, 0, 0, 32, 28, DMA);
}

void updateLevel(u8 level)
{
    if (level == 1)
    {
        if (global_counter == 2) // 2 is the earliest global counter for spawning stuff
        {

            levelObject.map_height = 16 * 1 - 16; // 16 tiles per screen height
            levelObject.map_width = 20 * 1 - 20;  // 20 tiles per screen width
            levelObject.beastmode_chance = 100;
            levelObject.beastmode_time_limit = 200;
            levelObject.enemy_shot_chance = 100; // percent
            levelObject.doors_closed_limit = 3;
            levelObject.shuts_to_seal = 3; // 3 times to seal
            // door position fix
            applyDoorOffsets();
            // player spawn
            player.x = SCREEN_X_OFFSET;
            player.y = SCREEN_Y_OFFSET;
            // initDoor(80 + SCREEN_X_OFFSET, 72 + SCREEN_Y_OFFSET);
            // initDoor(64 + SCREEN_X_OFFSET + 20 * 8, 16 + SCREEN_Y_OFFSET);
            // initDoor(16 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET);
            // initDoor(16 + SCREEN_X_OFFSET, 64 + SCREEN_Y_OFFSET);
            // initEnemy(ENEMY_TYPE_DEMON, 50, 50);
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
        if (doors_closed == levelObject.doors_closed_limit)
        {
            initLevel(current_level + 1);
        }
    }
    else if (level == 2)
    {
        if (global_counter == 2) // 2 is the earliest global counter for spawning stuff
        {

            levelObject.map_height = 16 * 1 - 16; // 16 tiles per screen height
            levelObject.map_width = 20 * 2 - 20;  // 20 tiles per screen width
            levelObject.beastmode_chance = 100;
            levelObject.beastmode_time_limit = 200;
            levelObject.enemy_shot_chance = 100; // percent
            levelObject.doors_closed_limit = 3;
            levelObject.shuts_to_seal = 3; // 3 times to seal

            // player spawn
            player.x = SCREEN_X_OFFSET;
            player.y = SCREEN_Y_OFFSET;
            // on screen 1
            initDoor(80 + SCREEN_X_OFFSET, 72 + SCREEN_Y_OFFSET, 0, 0);
            // on screen 2
            initDoor(64 + SCREEN_X_OFFSET, 16 + SCREEN_Y_OFFSET, 20, 0);

            applyDoorOffsets();
        }
        else if (global_counter == 100)
        {
            // levelObject.beastmode_chance = 10;
            // levelObject.beastmode_time_limit = 200;
        }
    }
    else if (level == 3)
    {
        if (global_counter == 2) // 2 is the earliest global counter for spawning stuff
        {

            levelObject.map_height = 16 * 2 - 16; // 16 tiles per screen height
            levelObject.map_width = 20 * 2 - 20;  // 20 tiles per screen width
            // levelObject.collision_map = [];
            levelObject.beastmode_chance = 100;
            levelObject.beastmode_time_limit = 200;
            levelObject.enemy_shot_chance = 60; // percent
            // player spawn
            player.x = 50 + SCREEN_X_OFFSET;
            player.y = 50 + SCREEN_Y_OFFSET;

            // initEnemy(ENEMY_TYPE_DEMON, 50, 50);
        }
        else if (global_counter == 300)
        {
            // levelObject.beastmode_chance = 10;
            // levelObject.beastmode_time_limit = 200;

            // initEnemy(ENEMY_TYPE_DEMON, 50, 50);
        }
    }
    if (UPDATE_SCROLL)
    {
        UPDATE_SCROLL = false;
        MAP_X += SCROLL_X;
        MAP_Y += SCROLL_Y;
        SCROLL_X = 0;
        SCROLL_Y = 0;
        VDP_setTileMapEx(BG_B, currentMap, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 6, 5, MAP_X, MAP_Y, 20, 16, DMA);
    }
}