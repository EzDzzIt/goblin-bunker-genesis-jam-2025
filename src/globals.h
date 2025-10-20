#pragma once

#include <genesis.h>

#define SCREEN_X_OFFSET (256 - 160) * 0.5 // WE ARE PLAYING A GAMEBOY GAME HERE LMAO
#define SCREEN_Y_OFFSET (224 - 144) * 0.5
#define SCREEN_X_END (256 - SCREEN_X_OFFSET) // WE ARE PLAYING A GAMEBOY GAME HERE LMAO
#define SCREEN_Y_END (224 - SCREEN_Y_OFFSET)

typedef struct
{
    u16 joy;
    u16 changed;
    u16 state;
} InputType;

extern InputType input;