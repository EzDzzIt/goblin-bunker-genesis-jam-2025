#include "level.h"
#include "globals.h"
#include "resources.h"

struct objectData door_array[1];

struct objectData initObject(u8 type, u8 x, u8 y)
{

    struct objectData object;
    if (type == 0)
    {
        object.sprite = SPR_addSprite(&door_sprite, 0, 0, TILE_ATTR(PAL0, 0, FALSE, FALSE));
        object.x = x;
        object.y = y;
    }
    return object;
}

void updateLevel()
{
    u8 i = 0;
    // for (i = 0; 1; i++)
    {
        SPR_setPosition(door_array[i].sprite, door_array[i].x, door_array[i].y);
        // SPR_setHFlip(player.sprite, player.hflip);
    }
}