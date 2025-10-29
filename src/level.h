#pragma once

#include <genesis.h>

// universal entity properties

struct objectData
{
    bool active;
    Sprite *sprite;
    s16 x;
    s16 y;
    bool hflip;
};

// door stuff

#define DOOR_OBJECT 0
#define DOOR_OPENING_ANIM 1
#define DOOR_SEALED_ANIM 2
#define DOOR_WIDTH 8
#define DOOR_HEIGHT 16
#define MAX_DOORS 12

struct doorData
{
    struct objectData data;
    bool beastmode;
    Sprite *beastmode_sprite;
    s16 beastmode_x;
    s16 beastmode_y;
    bool beastmode_hflip;
    u16 beastmode_counter;
    u8 shut_count;
    bool sealed;
    u8 close_cooldown;
    u8 push_x;
    u8 push_y;
};

void initDoor(u8 x, u8 y, u8 push_x, u8 push_y);
void toggleDoorBeastmode(u8 index);
void updateDoors();
void shutDoor(u8 index);
void applyObjectOffsets();

extern struct doorData door_array[];

// other level entities (keys, idols, hidden blocks)

#define MAX_OBJECTS 12
#define OBJECT_TYPE_IDOL 0
#define OBJECT_TYPE_KEY 1

// level objects

struct levelObjectData
{
    struct objectData data;
    u8 object_type;
    u8 push_x;
    u8 push_y;
};

extern struct levelObjectData level_object_array[];
void initObject(u8 object_type, u8 x, u8 y, u8 push_x, u8 push_y);
void updateObjects();
void pickupObject(u8 index);

// actual level logic

struct levelData
{
    u8 beastmode_chance;
    u16 beastmode_time_limit;
    u8 enemy_shot_chance;
    u8 map_width;
    u8 map_height; // tiles
    u8 doors_closed_limit;
    u8 shuts_to_seal; // how many times you have to shgut a door to seal it in a level
    u8 level_timer_max;
};

extern struct levelData level_data;

void initLevel(u8 level);
void updateLevel(u8 level);
