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
};

void initDoor(u8 x, u8 y);
void toggleDoorBeastmode(u8 index);
void updateDoors();

extern struct doorData door_array[];

// actual level logic

struct levelData
{
    u8 beastmode_chance;
    u16 beastmode_time_limit;
    u8 enemy_shot_chance;
    u8 map_width;
    u8 map_height; // tiles
    u8 doors_closed_limit;
};

extern struct levelData levelObject;

void initLevel(u8 level);
void updateLevel(u8 level);
