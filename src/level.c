#include "level.h"
#include "globals.h"
#include "resources.h"

struct doorData door_array[1];

struct doorData initDoor(u8 x, u8 y)
{

    struct doorData door;

    door.data.sprite = SPR_addSprite(&door_sprite, 0, 0, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    door.data.x = x;
    door.data.y = y;

    SPR_setAnim(door.data.sprite, DOOR_OPENING_ANIM);
    return door;
}

void updateLevel()
{
    u8 i = 0;
    // for (i = 0; 1; i++)
    {
        SPR_setPosition(door_array[i].data.sprite, door_array[i].data.x, door_array[i].data.y);
        // SPR_setHFlip(player.sprite, player.hflip);
    }
}