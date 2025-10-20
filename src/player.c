#include "player.h"
#include "globals.h"
#include "resources.h"

struct playerData player;

void initPlayer()
{
    player.sprite = SPR_addSprite(&player_sprite, 0, 0, TILE_ATTR(PAL0, 0, FALSE, FALSE));
}

void updatePlayer()
{
    checkInput();
    SPR_setPosition(player.sprite, player.x, player.y);
    SPR_setHFlip(player.sprite, player.hflip);
}

void checkInput()
{

    u16 joy = input.joy;
    u16 state = input.state;
    u16 changed = input.changed;

    if (joy == JOY_1)
    {
        if (state & BUTTON_LEFT)
        {
            player.x -= 1;
            player.hflip = true;
        }
        if (state & BUTTON_RIGHT)
        {
            player.x += 1;
            player.hflip = false;
        }
        if (state & BUTTON_UP)
        {
            player.y -= 1;
        }
        if (state & BUTTON_DOWN)
        {
            player.y += 1;
        }
    }
}