#include "player.h"
#include "globals.h"
#include "resources.h"
#include "level.h"
#include "enemy.h"

struct playerData player;

void debug_player_info_print()
{
    char buffer[16];
    sprintf(buffer, "%d", player.hp);
    VDP_drawText(buffer, 0, 0);
}

void initPlayer()
{
    player.sprite = SPR_addSprite(&player_sprite, SCREEN_X_OFFSET, SCREEN_Y_OFFSET, TILE_ATTR(PAL0, 0, FALSE, FALSE));
    player.speed = 1.8;
    player.x = SCREEN_X_OFFSET;
    player.y = SCREEN_Y_OFFSET;
    player.hp = 3;
    SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
}

void updatePlayer()
{
    // checkInput();
    // apply velocity
    // check collide
    // is the player dead?
    if (player.hp <= 0)
    {
        game_state = GAME_STATE_OVER;
        global_counter = 0;
    }
    player.x += player.velocity.x;
    player.y += player.velocity.y;

    // stuff that stops u
    if (player.x >= SCREEN_X_END - PLAYER_WIDTH)
    {
        player.x -= player.velocity.x;
        player.x = SCREEN_X_END - PLAYER_WIDTH;
    }
    else if (player.x <= SCREEN_X_OFFSET)
    {
        player.x -= player.velocity.x;
        player.x = SCREEN_X_OFFSET;
    }
    if (player.y >= SCREEN_Y_END - PLAYER_HEIGHT)
    {
        player.y -= player.velocity.y;
        player.y = SCREEN_Y_END - PLAYER_HEIGHT;
    }
    else if (player.y <= SCREEN_Y_OFFSET)
    {
        player.y -= player.velocity.y;
        player.y = SCREEN_Y_OFFSET;
    }
    u8 i = 0;
    bool collided = false;
    bool hurt = false;
    // cycle through stuff for things that stop movement
    for (i = 0; i < 5; i++)
    {
        if (door_array[i].data.active)
        {
            if (collision_check(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, door_array[i].data.x + 1, door_array[i].data.y + 3, DOOR_WIDTH - 4, DOOR_HEIGHT - 4))
            {
                collided = true;
                break;
            }
        }
    }
    // cycle through stuff that hurts u, only if you are not invincible &|| warping
    if (player.hurt_cooldown <= 0 && player.warp_cooldown <= 80)
    {
        for (i = 0; i < 5; i++)
        {
            if (enemy_array[i].data.active)
            {
                if (collision_check(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, enemy_array[i].data.x, enemy_array[i].data.y, enemy_array[i].width, enemy_array[i].height))
                {
                    player.hurt_cooldown = PLAYER_HURT_COOLDOWN;
                    player.hp -= 1;
                }
            }
            if (bullet_array[i].data.active)
            {
                if (collision_check(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, bullet_array[i].data.x, bullet_array[i].data.y, BULLET_WIDTH, BULLET_HEIGHT))
                {
                    player.hurt_cooldown = PLAYER_HURT_COOLDOWN;
                    player.hp -= 1;
                }
            }
        }
        if (player.hurt_cooldown > 0)
        {
            SPR_setAnim(player.sprite, PLAYER_ANIM_HURT);
        }
    }

    if (collided)
    {
        u8 teleport_correction = 0;
        if (player.last_input == BUTTON_A)
        {
            teleport_correction = PLAYER_TELEPORT_CORRECTION;
        }
        if (player.velocity.x > 0)
        {
            player.x -= player.velocity.x + 2 + teleport_correction;
        }
        else if (player.velocity.x < 0)
        {
            player.x -= player.velocity.x - 2 - teleport_correction;
        }
        if (player.velocity.y > 0)
        {
            player.y -= player.velocity.y + 2 + teleport_correction;
        }
        else if (player.velocity.y < 0)
        {
            player.y -= player.velocity.y - 2 - teleport_correction;
        }

        player.move_cooldown = PLAYER_MOVE_COOLDOWN;
        player.velocity.y = 0;
        player.velocity.x = 0;
    }

    // update sprite engine
    SPR_setPosition(player.sprite, player.x, player.y);
    SPR_setHFlip(player.sprite, player.hflip);

    // update timers
    if (player.warp_cooldown > 0)
    {
        player.warp_cooldown -= 1;
        if (player.warp_cooldown == 60)
        {
            SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
        }
    }
    if (player.move_cooldown > 0)
    {
        player.move_cooldown -= 1;
    }
    if (player.hurt_cooldown > 0)
    {
        player.hurt_cooldown -= 1;
        if (player.hurt_cooldown == 25)
        {
            SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
        }
    }
    if (player.hp <= 0)
    {
        game_state = GAME_STATE_OVER;
    }
}

void checkInput()
{

    u16 joy = input.joy;
    u16 state = input.state;
    u16 changed = input.changed;

    if (joy == JOY_1)
    {
        if (game_state == GAME_STATE_GAME) // gameplay controls
        {
            u8 movement_mask = state & 0b1111;
            // u8 action_mask = state & 0b000011110000;
            // BUTTON_A
            // BUTTON_B
            // BUTTON_C
            // BUTTON_START
            player.velocity.x = 0;
            player.velocity.y = 0;
            player.last_input = 0;
            if (player.move_cooldown <= 0)
            {
                if (state & BUTTON_A && player.warp_cooldown <= 0)
                {

                    if (state & BUTTON_LEFT)
                    {
                        player.velocity.x -= (player.speed + 2); // keep momentum
                        // player.x -= 24;                    // warp 24 px
                        player.warp_cooldown = PLAYER_WARP_COOLDOWN; // cooldown
                        player.hflip = true;
                        SPR_setAnim(player.sprite, PLAYER_ANIM_TELEPORT);
                        player.last_input = BUTTON_A;
                    }
                    if (state & BUTTON_RIGHT)
                    {
                        player.velocity.x += (player.speed + 2);
                        // player.x += 24;                    // warp 24 px
                        player.warp_cooldown = PLAYER_WARP_COOLDOWN; // cooldown
                        player.hflip = false;
                        SPR_setAnim(player.sprite, PLAYER_ANIM_TELEPORT);
                        player.last_input = BUTTON_A;
                    }
                    if (state & BUTTON_UP)
                    {
                        player.velocity.y -= (player.speed + 2);
                        // player.y -= 24;
                        player.warp_cooldown = PLAYER_WARP_COOLDOWN; // cooldown
                        SPR_setAnim(player.sprite, PLAYER_ANIM_TELEPORT);
                        player.last_input = BUTTON_A;
                    }
                    if (state & BUTTON_DOWN)
                    {
                        player.velocity.y += (player.speed + 2);
                        // player.y += 24;
                        player.warp_cooldown = PLAYER_WARP_COOLDOWN; // cooldown
                        SPR_setAnim(player.sprite, PLAYER_ANIM_TELEPORT);
                        player.last_input = BUTTON_A;
                    }
                }
                else
                {
                    if (state & BUTTON_LEFT)
                    {
                        player.velocity.x -= player.speed;
                        player.hflip = true;
                        player.last_input = BUTTON_LEFT;
                    }
                    if (state & BUTTON_RIGHT)
                    {
                        player.velocity.x += player.speed;
                        player.hflip = false;
                        player.last_input = BUTTON_RIGHT;
                    }
                    if (state & BUTTON_UP)
                    {
                        player.velocity.y -= player.speed;
                        player.last_input = BUTTON_UP;
                    }
                    if (state & BUTTON_DOWN)
                    {
                        player.velocity.y += player.speed;
                        player.last_input = BUTTON_DOWN;
                    }
                }
            }
        }
        else if (game_state == GAME_STATE_TITLE)
        {
            if (state & BUTTON_START && !title_skip)
            {
                title_skip = true;
            }
        }
    }
}