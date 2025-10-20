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