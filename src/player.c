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
    player.x += 1;
    player.y += 1;
    SPR_setPosition(player.sprite, player.x, player.y);
}

void checkInput()
{

    u16 joy = input.joy;
    u16 state = input.state;
    u16 changed = input.changed;
}