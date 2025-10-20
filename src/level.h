#pragma once

#include <genesis.h>

#define DOOR_OBJECT 0
#define DOOR_OPENING_ANIM 1

struct objectData
{
    Sprite *sprite;
    u8 x;
    u8 y;
    bool hflip;
};

struct doorData
{
    struct objectData data;
};

struct doorData initDoor(u8 x, u8 y);

void updateLevel();

extern struct doorData door_array[];