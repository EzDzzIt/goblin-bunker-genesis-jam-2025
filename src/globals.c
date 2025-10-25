#include <genesis.h>
#include "resources.h"
#include "globals.h"
#include "player.h"
#include "level.h"
#include "enemy.h"
#include "game.h"
#include "spell.h"

// global variables

InputType input;

// enum game_state_enum game_state = GAME_STATE_GAME;
enum game_state_enum game_state = GAME_STATE_TITLE;

u8 title_counter = 0;
u16 global_counter = 0;
bool title_skip = false;
u16 score = 0;
s8 SCROLL_X = 0;
s8 SCROLL_Y = 0;
u8 MAP_X = 0;
u8 MAP_Y = 0; // these represent the region of the tilemap we are in
bool UPDATE_SCROLL = FALSE;

bool collision_check(f16 x, f16 y, u8 w, u8 h, f16 x2, f16 y2, u8 w2, u8 h2)
{
    if (x > x2 + w2)
    {
        return false;
    }
    if (x + w < x2)
    {
        return false;
    }
    if (y + h < y2)
    {
        return false;
    }
    if (y > y2 + h2)
    {
        return false;
    }
    if (x < x2 + w2 && x + w > x2 && y < y2 + h2 && y + h > y2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void setSprite(Sprite *sprite, fix16 x, fix16 y)
{
    SPR_setPosition(sprite, x - SCROLL_X * 8, y - SCROLL_Y * 8);
}

void clear_graphics(bool stop_music)
{
    // start by cleaning up old level memory
    VDP_clearTileMap(BG_A, 0, 400, TRUE);
    VDP_clearTileMap(BG_B, 0, 400, TRUE);
    // VDP_clearTileMap(BG_B, 0, 200, TRUE);
    VDP_clearTileMapRect(BG_A, 0, 0, 32, 28);
    VDP_clearTileMapRect(BG_B, 0, 0, 32, 28);
    VDP_clearSprites();
    SPR_clear();
    SPR_reset();
    SPR_defragVRAM();
    if (stop_music)
    {
        XGM2_stop();
    }
}

void reset_globals()
{
    score = 0; // reset score just in case
    u8 i = 0;
    for (i = 0; i < 2; i++)
    {
        sacred_ground_array[i].lifetime = 0;
        sacred_ground_array[i].data.active = false;
    }
}