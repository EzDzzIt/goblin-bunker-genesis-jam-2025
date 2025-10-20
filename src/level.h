#pragma once

#include <genesis.h>

struct objectData
{
    Sprite *sprite;
    u8 x;
    u8 y;
    bool hflip;
};

struct objectData initObject(u8 type, u8 x, u8 y);

void updateLevel();

extern struct objectData door_array[1];