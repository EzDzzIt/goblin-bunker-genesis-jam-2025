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
extern u8 doors_closed;
extern u8 secrets_found;
extern u8 enemies_killed;
extern u8 total_doors_closed;
extern u16 total_enemies_killed;
extern s8 SCROLL_X;
extern s8 SCROLL_Y;
extern u8 MAP_X;
extern u8 MAP_Y;
extern void *current_map_data;
extern bool UPDATE_SCROLL;
extern TileMap *currentMap;
extern u8 current_level;

extern bool collision_check(s16 x, s16 y, u8 width, u8 height, s16 x2, s16 y2, u8 width2, u8 height2);
extern void clear_graphics(bool stop_music);
extern void reset_globals();
extern void setSprite(Sprite *sprite, s16 x, s16 y);

// level tile data
extern const u8 level_1_map_data[16][20];
extern const u8 level_2_map_data[16][20];
extern const u8 level_3_map_data[16][20];
extern const u8 level_4_map_data[32][40];
extern const u8 level_5_map_data[32][40];
extern const u8 level_6_map_data[32][40];
extern const u8 level_7_map_data[32][40];
extern const u8 level_8_map_data[32][40];
extern const u8 level_9_map_data[32][40];
extern const u8 level_10_map_data[32][40];
extern const u8 level_q1_map_data[32][40];
