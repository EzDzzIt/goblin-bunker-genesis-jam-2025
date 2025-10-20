#include "player.h"
#include "globals.h"
#include "resources.h"

struct playerData player;

void initPlayer()
{
    player.sprite = SPR_addSprite(&player_sprite, SCREEN_X_OFFSET, SCREEN_Y_OFFSET, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    player.speed = 1.8;
    player.x = SCREEN_X_OFFSET;
    player.y = SCREEN_Y_OFFSET;
    // player.last_input = 0x0000; // continuous pressed down
    SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
}

void updatePlayer(u16 time)
{
    // checkInput();
    // apply velocity
    // check collide

    player.x += player.velocity.x;
    player.y += player.velocity.y;
    if (player.x >= SCREEN_X_END - PLAYER_WIDTH || player.x <= SCREEN_X_OFFSET)
    {
        player.x -= player.velocity.x;
    }
    if (player.y >= SCREEN_Y_END - PLAYER_HEIGHT || player.y <= SCREEN_Y_OFFSET)
    {
        player.y -= player.velocity.y;
    }
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