#include "level.h"
#include "globals.h"
#include "resources.h"
#include "player.h"
#include "enemy.h"

// doors

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
            SPR_setAnim(door.data.sprite, 0);
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
                if (door_array[i].beastmode_counter >= level_data.beastmode_time_limit)
                {
                    // toggleDoorBeastmode(i);
                    door_array[i].beastmode_counter = 0;

                    if ((random() % (100 - 1 + 1)) + 1 <= 15)
                    {
                        initEnemy(ENEMY_TYPE_DEMON, door_array[i].data.x, door_array[i].data.y, door_array[i].push_x, door_array[i].push_y);
                    }
                    else
                    {
                        initEnemy(ENEMY_TYPE_EYE, door_array[i].data.x, door_array[i].data.y, door_array[i].push_x, door_array[i].push_y);
                    }
                }
            }
            else
            {
                if (!door_array[i].sealed)
                {
                    if ((random() % (1000 - 1 + 1)) + 1 <= level_data.beastmode_chance)
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
    XGM2_playPCM(wav_door_sealed, sizeof(wav_door_sealed), SOUND_PCM_CH_AUTO);
    if (door_array[index].shut_count >= level_data.shuts_to_seal)
    {
        // seal door
        door_array[index].sealed = true;
        SPR_setAnim(door_array[index].data.sprite, DOOR_SEALED_ANIM);
        doors_closed += 1;
        total_doors_closed += 1;
        score += 50;
    }
    else
    {

        door_array[index].shut_count += 1;
        door_array[index].close_cooldown = 60;
    }
}

struct levelObjectData level_object_array[MAX_OBJECTS];

void initObject(u8 object_type, u8 x, u8 y, u8 push_x, u8 push_y, u8 sub_type)
{
    u8 i = 0;
    for (i = 0; i < MAX_OBJECTS; i++)
    {
        if (!level_object_array[i].data.active)
        {
            struct levelObjectData obj;
            obj.data.x = x;
            obj.data.y = y;
            obj.object_type = object_type;
            obj.sub_type = sub_type;
            if (object_type == OBJECT_TYPE_IDOL)
            {
                obj.data.sprite = SPR_addSprite(&idol_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
            }
            else if (object_type == OBJECT_TYPE_KEY)
            {
                obj.data.sprite = SPR_addSprite(&key_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
            }
            else if (object_type == OBJECT_TYPE_PORTAL)
            {
                obj.data.sprite = SPR_addSprite(&portal_sprite, x, y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
            }
            obj.data.active = true;
            obj.push_x = push_x;
            obj.push_y = push_y;
            level_object_array[i] = obj;
            break;
        }
    }
}

void updateObjects()
{
    u8 i = 0;
    for (i = 0; i < MAX_OBJECTS; i++)
    {
        if (level_object_array[i].data.active)
        {
            if (UPDATE_SCROLL)
            {
                level_object_array[i].data.x -= SCROLL_X * 8;
                level_object_array[i].data.y -= SCROLL_Y * 8;
            }
            SPR_setPosition(level_object_array[i].data.sprite, level_object_array[i].data.x, level_object_array[i].data.y);
        }
    }
}

void pickupObject(u8 index)
{

    if (level_object_array[index].object_type == OBJECT_TYPE_IDOL)
    {
        player.hp += 1;
        secrets_found += 1;
        SPR_releaseSprite(level_object_array[index].data.sprite);
        level_object_array[index].data.active = false;
    }
    else if (level_object_array[index].object_type == OBJECT_TYPE_KEY)
    {
        has_key = TRUE;
        SPR_releaseSprite(level_object_array[index].data.sprite);
        level_object_array[index].data.active = false;
    }
    else if (level_object_array[index].object_type == OBJECT_TYPE_PORTAL)
    {
        // warp to location saved in current level data on next player update
        player.portal_warp = level_object_array[index].sub_type;
    }
}

void applyObjectOffsets()
{
    u8 i = 0;
    for (i = 0; i < MAX_DOORS; i++)
    {
        if (door_array[i].data.active)
        {
            door_array[i].data.x += (door_array[i].push_x - MAP_X) * 8;
            door_array[i].data.y += (door_array[i].push_y - MAP_Y) * 8;
            SPR_setPosition(door_array[i].data.sprite, door_array[i].data.x, door_array[i].data.y);
        }
    }
    for (i = 0; i < MAX_OBJECTS; i++)
    {
        if (level_object_array[i].data.active)
        {
            level_object_array[i].data.x += (level_object_array[i].push_x - MAP_X) * 8;
            level_object_array[i].data.y += (level_object_array[i].push_y - MAP_Y) * 8;
            SPR_setPosition(level_object_array[i].data.sprite, level_object_array[i].data.x, level_object_array[i].data.y);
        }
    }
}

// level init

struct levelData level_data;

void initLevel(u8 level)
{
    // clear_graphics(FALSE);

    current_level = level;
    reset_globals();
    SPR_defragVRAM();

    if (level == 1)
    {
        currentMap = &level_1_map;
        current_map_data = level_1_map_data;
        current_map_data_columns = 20;
        // init level data
        level_data.map_height = 16 * 1 - 16; // 16 tiles per screen height
        level_data.map_width = 20 * 1 - 20;  // 20 tiles per screen width
        level_data.beastmode_chance = 45;
        level_data.beastmode_time_limit = 300;
        level_data.enemy_shot_chance = 100; // percent
        level_data.doors_closed_limit = 1;  // seal one door to win the level
        level_data.shuts_to_seal = 2;       // shut each door 3 times to seal
        level_data.level_timer_max = 30;    // seconds on the clock
        level_timer = level_data.level_timer_max;
        // player spawn
        player.x = SCREEN_X_OFFSET + 8;
        player.y = SCREEN_Y_OFFSET + 8;
        // player.hp += 1; // heal a bit each round
        // level objects
        initDoor(80 + SCREEN_X_OFFSET, 72 + SCREEN_Y_OFFSET, 0, 0);
        initEnemy(ENEMY_TYPE_SECRET, SCREEN_X_OFFSET, 14 * 8 + SCREEN_Y_OFFSET, 0, 0);
        // apply object offsets to other screens if needed
        applyObjectOffsets();
    }
    else if (level == 2)
    {
        currentMap = &level_2_map;
        current_map_data = level_2_map_data;
        current_map_data_columns = 20;

        level_data.map_height = 16 * 1 - 16; // 16 tiles per screen height
        level_data.map_width = 20 * 1 - 20;  // 20 tiles per screen width
        level_data.beastmode_chance = 45;
        level_data.beastmode_time_limit = 200;
        level_data.enemy_shot_chance = 100; // percent
        level_data.doors_closed_limit = 1;
        level_data.shuts_to_seal = 2; // 3 times to seal

        // player spawn
        player.x = SCREEN_X_OFFSET + 2 * 8;
        player.y = SCREEN_Y_OFFSET + 11 * 8;
        // on screen 1
        initDoor(80 + SCREEN_X_OFFSET, 32 + SCREEN_Y_OFFSET, 0, 0);

        applyObjectOffsets();
    }
    else if (level == 3)
    {
        currentMap = &level_3_map;
        current_map_data = level_3_map_data;
        current_map_data_columns = 20;
        level_data.map_height = 16 * 1 - 16; // 16 tiles per screen height
        level_data.map_width = 20 * 1 - 20;  // 20 tiles per screen width
        level_data.beastmode_chance = 45;
        level_data.beastmode_time_limit = 300;
        level_data.enemy_shot_chance = 100; // percent
        level_data.doors_closed_limit = 1;  // seal one door to win the level
        level_data.shuts_to_seal = 2;       // shut each door 3 times to seal
        level_data.level_timer_max = 30;    // seconds on the clock
        level_timer = level_data.level_timer_max;
        // player spawn
        player.x = SCREEN_X_OFFSET + 4 * 8;
        player.y = SCREEN_Y_OFFSET + 10 * 8;
        // player.hp += 1; // heal a bit each round
        // level objects
        initDoor(17 * 8 + SCREEN_X_OFFSET, 8 * 8 + SCREEN_Y_OFFSET, 0, 0);
        initObject(OBJECT_TYPE_KEY, SCREEN_X_OFFSET + 17 * 8, SCREEN_Y_OFFSET + 2 * 8, 0, 0, 0);
        // apply object offsets to other screens if needed
        applyObjectOffsets();
    }
    else if (level == 4)
    {
        currentMap = &level_4_map;
        current_map_data = level_4_map_data;
        current_map_data_columns = 40;
        level_data.map_height = 16 * 1 - 16; // 16 tiles per screen height
        level_data.map_width = 20 * 2 - 20;  // 20 tiles per screen width
        MAP_X = 0;
        MAP_Y = 0;
        level_data.beastmode_chance = 45;
        level_data.beastmode_time_limit = 250;
        level_data.enemy_shot_chance = 100;                     // percent
        level_data.doors_closed_limit = 4;                      // seal n doors to win the level
        level_data.shuts_to_seal = 2;                           // shut each door 3 times to seal
        level_data.level_timer_max = 50;                        // seconds on the clock
        level_data.x_warp_points[0] = 15 * 8 + SCREEN_X_OFFSET; // portal 0 warp
        level_data.y_warp_points[0] = 3 * 8 + SCREEN_Y_OFFSET;
        level_timer = level_data.level_timer_max;
        // player spawn
        player.x = SCREEN_X_OFFSET + 3 * 8;
        player.y = SCREEN_Y_OFFSET + 8 * 8;
        // player.hp += 1; // heal a bit each round
        // level objects
        initDoor(15 * 8 + SCREEN_X_OFFSET, 8 * 8 + SCREEN_Y_OFFSET, 20, 0);

        // apply object offsets to other screens if needed
        applyObjectOffsets();
    }
    else if (level == 5)
    {
        currentMap = &level_5_map;
        current_map_data = level_5_map_data;
        current_map_data_columns = 20;
        level_data.map_height = 16 * 1 - 16; // 16 tiles per screen height
        level_data.map_width = 20 * 1 - 20;  // 20 tiles per screen width
        level_data.beastmode_chance = 45;
        level_data.beastmode_time_limit = 250;
        level_data.enemy_shot_chance = 100; // percent
        level_data.doors_closed_limit = 1;  // seal n doors to win the level
        level_data.shuts_to_seal = 5;       // shut each door 3 times to seal
        level_data.level_timer_max = 200;   // seconds on the clock

        level_timer = level_data.level_timer_max;
        // player spawn
        player.x = SCREEN_X_OFFSET + 8 * 8;
        player.y = SCREEN_Y_OFFSET + 10 * 8;
        // player.hp += 1; // heal a bit each round
        // level objects

        // apply object offsets to other screens if needed
        applyObjectOffsets();
    }
    // else if (level == 6)
    // {
    //     currentMap = &level_6_map;
    //     current_map_data = level_6_map_data;
    //     current_map_data_columns = 20;
    // }

    SYS_disableInts();
    VDP_loadTileSet(&level_tileset, 0, DMA);
    VDP_setTileMapEx(BG_B, currentMap, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 6, 5, SCROLL_X, SCROLL_Y, 20, 16, DMA);
    VDP_loadTileSet(&border_tileset, level_tileset.numTile, DMA);
    VDP_setTileMapEx(BG_A, &border_image, TILE_ATTR_FULL(PAL3, 1, FALSE, FALSE, level_tileset.numTile), 0, 0, 0, 0, 32, 28, DMA);
    SYS_enableInts();
}

void updateLevel(u8 level)
{
    if (level == 4)
    {
        if (doors_closed == 1 && level_state == 0)
        {

            initDoor(3 * 8 + SCREEN_X_OFFSET, 7 * 8 + SCREEN_Y_OFFSET, 20, 0);
            level_state += 1;
        }
        else if (doors_closed == 2 && level_state == 1)
        {
            initDoor(16 * 8 + SCREEN_X_OFFSET, 3 * 8 + SCREEN_Y_OFFSET, 20, 0);
            initDoor(7 * 8 + SCREEN_X_OFFSET, 4 * 8 + SCREEN_Y_OFFSET, 20, 0);
            initObject(OBJECT_TYPE_PORTAL, 14 * 8 + SCREEN_X_OFFSET, 12 * 8 + SCREEN_Y_OFFSET, 20, 0, 0);
            level_state += 1;
        }
    }
    else if (level == 5)
    {
        if (enemies_killed < 7)
        {
            if (global_counter % 120 == 0)
            {
                randomEnemySpawn();
            }
        }
        else if (enemies_killed < 15)
        {
            if (global_counter % 100 == 0)
            {
                randomEnemySpawn();
            }
        }
        else if (enemies_killed == 15 && level_state == 0)
        {
            initDoor(9 * 8 + SCREEN_X_OFFSET, 6 * 8 + SCREEN_Y_OFFSET, 0, 0);
            level_state += 1;
        }
        else if (level_state == 1)
        {
            if (global_counter % 300 == 0)
            {
                randomEnemySpawn();
            }
        }
    }

    if (global_counter % 60 == 0)
    {
        level_timer -= 1;
        if (level_timer <= 0)
        {
            player.hp = 0; // you have died
        }
    }
    if (UPDATE_SCROLL)
    {
        UPDATE_SCROLL = false;
        MAP_X += SCROLL_X;
        MAP_Y += SCROLL_Y;
        SCROLL_X = 0;
        SCROLL_Y = 0;
        SYS_disableInts();
        VDP_setTileMapEx(BG_B, currentMap, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 6, 5, MAP_X, MAP_Y, 20, 16, DMA);
        SYS_enableInts();
    }
    if (doors_closed >= level_data.doors_closed_limit)
    {
        // go to the next level
        global_counter = 0;
        score += level_timer;
        game_state = GAME_STATE_TRANSITION;
    }
}