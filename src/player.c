#include "player.h"
#include "globals.h"
#include "resources.h"

struct playerData player;

void initPlayer()
{
    player.sprite = SPR_addSprite(&player_sprite, 0, 0, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    player.speed = 1.8;
    // player.last_input = 0x0000; // continuous pressed down
    SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
}

void updatePlayer()
{
    // checkInput();
    // apply velocity
    player.x += player.velocity.x;
    player.y += player.velocity.y;
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
        u8 movement_mask = state & 0b1111;
        player.velocity.x = 0;
        player.velocity.y = 0;
        if (movement_mask & BUTTON_LEFT)
        {
            player.velocity.x -= player.speed;
            player.hflip = true;
            // player.last_input = BUTTON_LEFT;
        }
        if (movement_mask & BUTTON_RIGHT)
        {
            player.velocity.x += player.speed;
            player.hflip = false;
            // player.last_input = BUTTON_RIGHT;
        }
        if (movement_mask & BUTTON_UP)
        {
            player.velocity.y -= player.speed;
            // player.last_input = BUTTON_UP;
        }
        if (movement_mask & BUTTON_DOWN)
        {
            player.velocity.y += player.speed;
            // player.last_input = BUTTON_DOWN;
        }
    }
}