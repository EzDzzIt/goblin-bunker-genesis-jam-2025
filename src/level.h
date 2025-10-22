#pragma once

#include <genesis.h>

// universal entity properties

struct objectData
{
    bool active;
    Sprite *sprite;
    u8 x;
    u8 y;
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
    u8 beastmode_x;
    u8 beastmode_y;
    bool beastmode_hflip;
};

void initDoor(u8 x, u8 y);

extern struct doorData door_array[];

// actual level logic

void initLevel(u8 id);
