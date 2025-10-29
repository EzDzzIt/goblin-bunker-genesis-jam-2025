#include "player.h"
#include "globals.h"
#include "resources.h"
#include "level.h"
#include "enemy.h"
#include "spell.h"

struct playerData player;

void player_info_print()
{
    // hp
    char buffer[4];
    sprintf(buffer, "%dHP", player.hp);
    VDP_drawText(buffer, 7, 21);
    // score
    char score_buffer[8];
    sprintf(score_buffer, "ti:%d", level_2_map_data[player.tile_y][player.tile_x]);
    VDP_clearTextArea(18, 21, 6, 1);
    VDP_drawText(score_buffer, 18, 21);
    // doors left
    char door_buffer[2];
    sprintf(door_buffer, "DOORS:%d", level_data.doors_closed_limit - doors_closed);
    VDP_drawText(door_buffer, 7, 22);
    // warp
    if (player.warp_cooldown <= 0)
    {
        // char warp_buffer[4];
        // sprintf(warp_buffer, "%dHP", player.hp);
        VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, 1473 + 22), 11, 21, 1, 1); // index 1473 is A
    }
    else
    {
        VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, 0), 11, 21, 1, 1);
    }
    // seal spell
    if (player.sacred_cooldown <= 0)
    {
        // char warp_buffer[4];
        // sprintf(warp_buffer, "%dHP", player.hp);
        VDP_drawText("S", 12, 21);
    }
    else
    {
        VDP_clearText(12, 21, 1);
    }
}

void initPlayer()
{
    player.sprite = SPR_addSprite(&player_sprite, SCREEN_X_OFFSET, SCREEN_Y_OFFSET, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    player.speed = 1.8;
    player.x = SCREEN_X_OFFSET;
    player.y = SCREEN_Y_OFFSET;
    player.hp = 3;
    SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
}

// u8 (*level_1_map_data_pointer)[16][20];
// level_1_map_data_pointer = &level_1_map_data;

void updatePlayer()
{

    player_info_print();

    player.x += player.velocity.x;
    player.y += player.velocity.y;

    // cast a spell last frame?
    if (player.cast == SPELL_SACRED)
    {
        initSpell(SPELL_SACRED, player.x, player.y);
        player.cast = 0;
        XGM2_playPCM(wav_sacred, sizeof(wav_sacred), SOUND_PCM_CH_AUTO);
    }
    else if (player.cast == SPELL_SHOT)
    {
        initSpell(SPELL_SHOT, player.x, player.y);
        player.cast = 0;
        // SND_PCM_startPlay(wav_laser, sizeof(wav_laser), SOUND_PCM_RATE_13400, SOUND_PAN_CENTER, FALSE);
        XGM2_playPCM(wav_shot, sizeof(wav_shot), SOUND_PCM_CH_AUTO);
    }

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
    if (player.y >= SCREEN_Y_END - PLAYER_HEIGHT - 16) // adjust for bottom gb window layer
    {
        player.y -= player.velocity.y;
        player.y = SCREEN_Y_END - PLAYER_HEIGHT - 16; // adjust for bottom gb window layer
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
    for (i = 0; i < MAX_DOORS; i++)
    {
        if (door_array[i].data.active)
        {
            if (collision_check(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, door_array[i].data.x + 2, door_array[i].data.y + 4, DOOR_WIDTH - 4, DOOR_HEIGHT - 12))
            {
                collided = true;
                break;
            }
        }
    }
    // cycle through stuff that hurts u, only if you are not invincible &|| warping
    if (player.hurt_cooldown <= 0 && player.warp_cooldown <= 105)
    {
        for (i = 0; i < MAX_ENEMIES; i++)
        {
            if (enemy_array[i].data.active)
            {
                if (collision_check(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, enemy_array[i].data.x, enemy_array[i].data.y, enemy_array[i].width, enemy_array[i].height))
                {
                    player.hurt_cooldown = PLAYER_HURT_COOLDOWN;
                    player.hp -= 1;
                    XGM2_playPCM(wav_player_hurt, sizeof(wav_player_hurt), SOUND_PCM_CH_AUTO);
                    break;
                }
            }
        }
        for (i = 0; i < MAX_BULLETS; i++)
        {
            if (bullet_array[i].data.active)
            {
                if (collision_check(player.x + 2, player.y + 1, PLAYER_WIDTH - 4, PLAYER_HEIGHT - 6, bullet_array[i].data.x + 1, bullet_array[i].data.y + 1, BULLET_WIDTH - 2, BULLET_HEIGHT - 2))
                {
                    player.hurt_cooldown = PLAYER_HURT_COOLDOWN;
                    player.hp -= 1;
                    SPR_releaseSprite(bullet_array[i].data.sprite);
                    bullet_array[i].data.active = false;
                    break;
                }
            }
        }
    }
    // cycle through level objects
    for (i = 0; i < MAX_OBJECTS; i++)
    {
        if (level_object_array[i].data.active)
        {
            if (collision_check(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, level_object_array[i].data.x, level_object_array[i].data.y, 8, 8))
            {
                pickupObject(i);
                break;
            }
        }
    }

    // TILES DEBUG
    if (!(player.velocity.x == 0 && player.velocity.y == 0))
    {
        u8 tile_x;
        u8 tile_y;
        if (player.velocity.x > 0)
        {
            tile_x = (player.x - SCREEN_X_OFFSET + PLAYER_WIDTH) / 8 + MAP_X;
        }
        else if (player.velocity.x <= 0)
        {
            tile_x = (player.x - SCREEN_X_OFFSET + PLAYER_WIDTH) / 8 + MAP_X;
        }
        if (player.velocity.y > 0)
        {
            tile_y = (player.y - SCREEN_Y_OFFSET + PLAYER_HEIGHT) / 8 + MAP_Y;
        }
        else if (player.velocity.y <= 0)
        {
            tile_y = (player.y - SCREEN_Y_OFFSET + PLAYER_HEIGHT) / 8 + MAP_Y;
        }
        u8 tile_type = (level_1_map_data)[tile_y][tile_x];
        if ((tile_type >= 2 && tile_type <= 9))
        {
            collided = true;
        }
    }

    if (collided)
    {
        if (player.velocity.x > 0)
        {
            player.x -= player.velocity.x + 2; //+ teleport_correction;
        }
        else if (player.velocity.x < 0)
        {
            player.x -= player.velocity.x - 2; //- teleport_correction;
        }
        if (player.velocity.y > 0)
        {
            player.y -= player.velocity.y + 2; //+ teleport_correction;
        }
        else if (player.velocity.y < 0)
        {
            player.y -= player.velocity.y - 2; //- teleport_correction;
        }

        player.move_cooldown = PLAYER_MOVE_COOLDOWN;
        player.velocity.y = 0;
        player.velocity.x = 0;
    }

    // update sprite engine
    SPR_setPosition(player.sprite, player.x, player.y);
    SPR_setHFlip(player.sprite, player.hflip);

    // update timers
    bool cooldown_flag = false;
    if (player.warp_cooldown > 0)
    {
        cooldown_flag = true;
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
        cooldown_flag = true;
        if (player.hurt_cooldown == PLAYER_HURT_COOLDOWN && player.attack_cooldown <= 0)
        {
            // SPR_defragVRAM();
            SPR_setAnim(player.sprite, PLAYER_ANIM_HURT);
        }
        player.hurt_cooldown -= 1;
        if (player.hurt_cooldown % 12 == 0)
        {
            SPR_setPalette(player.sprite, PAL2);
        }
        else
        {
            SPR_setPalette(player.sprite, PAL1);
        }
        if (player.hurt_cooldown == 0)
        {
            SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
            SPR_setPalette(player.sprite, PAL1);
        }
    }

    // manually adjusting the time for the spellcasting animation
    if (player.attack_cooldown > 0)
    {
        cooldown_flag = true;
        if (player.attack_cooldown == PLAYER_ATTACK_COOLDOWN)
        {
            SPR_setAnim(player.sprite, PLAYER_ANIM_SHOT);
        }

        else if (player.attack_cooldown == 10)
        {
            SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
        }
        player.attack_cooldown -= 1;
        if (player.attack_cooldown == 0)
        {
            if (BUTTON_B & JOY_readJoypad(JOY_1))
            {
                initSpell(SPELL_SHOT, player.x, player.y);
                player.attack_cooldown = PLAYER_ATTACK_COOLDOWN;
                XGM2_playPCM(wav_shot, sizeof(wav_shot), SOUND_PCM_CH_AUTO);
            }
        }
    }
    // sacred ground
    if (player.sacred_cooldown > 0)
    {
        cooldown_flag = true;
        if (player.sacred_cooldown == PLAYER_SACRED_COOLDOWN)
        {
            SPR_setAnim(player.sprite, PLAYER_ANIM_SACRED);
        }

        else if (player.sacred_cooldown == PLAYER_SACRED_COOLDOWN - 60)
        {
            SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
        }
        player.sacred_cooldown -= 1;
    }
    // if nothing is in cooldown, make sure we are in idle
    if (!cooldown_flag)
    {
        SPR_setAnim(player.sprite, PLAYER_ANIM_IDLE);
    }
    // adjust final scroll position based on player
    if (player.y >= SCREEN_Y_END - 32 && MAP_Y < level_data.map_height) // adjust for window
    {
        player.y = SCREEN_Y_OFFSET + 2; // send player to top of next area
        SCROLL_Y = 16;                  // tileset needs to scroll up by 16 tiles
        UPDATE_SCROLL = TRUE;
    }
    else if (player.y < SCREEN_Y_OFFSET + 1)
    {
        if (MAP_Y <= 0)
        {
            SCROLL_Y = 0;
        }
        else
        {
            player.y = SCREEN_Y_END - 33;
            SCROLL_Y = -16;
            UPDATE_SCROLL = TRUE;
        }
    }
    if (player.x >= SCREEN_X_END - 8 && MAP_X < level_data.map_width) // adjust for window
    {
        player.x = SCREEN_X_OFFSET + 8; // send player to top of next area
        SCROLL_X = 20;                  // tileset needs to scroll up by 16 tiles
        UPDATE_SCROLL = TRUE;
    }
    else if (player.x < SCREEN_X_OFFSET + 9)
    {
        if (MAP_X <= 0)
        {
            SCROLL_X = 0;
        }
        else
        {
            player.x = SCREEN_X_END - 9;
            SCROLL_X = -20;
            UPDATE_SCROLL = TRUE;
        }
    }
    // // are we dead yet
    if (player.hp <= 0)
    {
        game_state = GAME_STATE_OVER;
        global_counter = 0; // set this here to make logic easier
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
            if (movement_mask > 0)
            {
                player.last_input = 0;
            }

            if (player.move_cooldown <= 0)
            {

                if (state & BUTTON_LEFT)
                {
                    player.velocity.x -= (player.speed);
                    player.hflip = true;
                    player.last_input = player.last_input | BUTTON_LEFT;
                }
                if (state & BUTTON_RIGHT)
                {
                    player.velocity.x += (player.speed);
                    player.hflip = false;
                    player.last_input = player.last_input | BUTTON_RIGHT;
                }
                if (state & BUTTON_UP)
                {
                    player.velocity.y -= (player.speed);
                    player.last_input = player.last_input | BUTTON_UP;
                }
                if (state & BUTTON_DOWN)
                {
                    player.velocity.y += (player.speed);
                    player.last_input = player.last_input | BUTTON_DOWN;
                }
            }
            // check for attack/spell
            if (state & BUTTON_C && changed & BUTTON_C)
            {
                if (player.warp_cooldown <= 0)
                {
                    initWarp();
                }
            }
            if (state & BUTTON_B && changed & BUTTON_B)
            {
                if (player.attack_cooldown <= 0)
                {
                    player.attack_cooldown = PLAYER_ATTACK_COOLDOWN;
                    player.cast = SPELL_SHOT;
                }
            }
            if (state & BUTTON_A && changed & BUTTON_A)
            {
                if (player.sacred_cooldown <= 0)
                {
                    player.sacred_cooldown = PLAYER_SACRED_COOLDOWN;
                    player.cast = SPELL_SACRED;
                }
            }
            // DEBUG
            if (state & BUTTON_START && changed & BUTTON_START)
            {
                // initLevel(current_level + 1);
            }
        }
        else if (game_state == GAME_STATE_TITLE || game_state == GAME_STATE_OVER || game_state == GAME_STATE_TRANSITION)
        {
            if (state & BUTTON_START && !title_skip)
            {
                title_skip = true;
            }
        }
    }
}