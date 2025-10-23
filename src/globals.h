#pragma once

#include <genesis.h>

#define SCREEN_X_OFFSET (256 - 160) * 0.5 // WE ARE PLAYING A GAMEBOY GAME HERE LMAO
#define SCREEN_Y_OFFSET (224 - 144) * 0.5
#define SCREEN_X_END (256 - SCREEN_X_OFFSET) // WE ARE PLAYING A GAMEBOY GAME HERE LMAO
#define SCREEN_Y_END (224 - SCREEN_Y_OFFSET)

// game states
enum game_state_enum
{
    GAME_STATE_TITLE,
    GAME_STATE_GAME,
    GAME_STATE_TRANSITION,
    GAME_STATE_OVER,
    GAME_STATE_WIN,
    GAME_STATE_PAUSE
};

// enum game_state_enum game_state = GAME_STATE_TITLE;

typedef struct
{
    u16 joy;
    u16 changed;
    u16 state;
} InputType;

extern InputType input;

extern enum game_state_enum game_state;

extern u8 title_counter;
extern u16 global_counter;
extern bool title_skip;
extern u16 score;

extern bool collision_check(f16 x, f16 y, u8 width, u8 height, f16 x2, f16 y2, u8 width2, u8 height2);
extern void clear_graphics(bool stop_music);
extern void reset_globals();