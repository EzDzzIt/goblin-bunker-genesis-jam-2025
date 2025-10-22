#include "globals.h"

// global variables

InputType input;

// enum game_state_enum game_state = GAME_STATE_GAME;
enum game_state_enum game_state = GAME_STATE_TITLE;

u8 title_counter = 0;
u16 global_counter = 0;
bool title_skip = false;

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
