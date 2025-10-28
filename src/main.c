#include <genesis.h>
#include "resources.h"
#include "globals.h"
#include "player.h"
#include "level.h"
#include "enemy.h"
#include "game.h"

// TODO: player spell cast, fix dash move, door state for closing or opening, level spawning logic, enemy follow logic, music

void inGameJoyEvent(u16 joy, u16 changed, u16 state);
void display_stats();

int main(bool resetType)
{
	if (!resetType)
	{
		SYS_hardReset();
	}

	while (TRUE)
	{
		global_counter += 1; // update this counter no matter what

		if (game_state == GAME_STATE_GAME)
		{
			if (global_counter >= 50000)
			{
				global_counter = 2;
			}
			if (global_counter == 1)
			{
			}
			else
			{
				updatePlayer();
				updateGame();
				updateLevel(current_level);
				SPR_update();
				display_stats();
			}
		}
		else if (game_state == GAME_STATE_PAUSE)
		{
			// SYS_doVBlankProcess();
		}
		else if (game_state == GAME_STATE_TITLE)
		{

			if (title_counter == 0) // sgdk logo
			{

				if (global_counter >= 300 || title_skip) // time in frames for first title drop
				{
					title_skip = false;
					clear_graphics(FALSE);
					title_counter += 1;
					global_counter = 0; // reset this to 0 to time the next section
				}
				else if (global_counter == 1)
				{
					// ALL INITIALIZATION
					VDP_setScreenWidth256();
					VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_COLUMN);
					SPR_init();
					JOY_init();
					JOY_setEventHandler(inGameJoyEvent);
					PAL_setPalette(PAL0, palette_0.data, DMA);
					PAL_setPalette(PAL1, palette_1.data, DMA);
					PAL_setPalette(PAL2, palette_2.data, DMA);
					PAL_setPalette(PAL3, palette_3.data, DMA);
					VDP_setTextPalette(PAL1);
					VDP_setBackgroundColor(5); // change this per level?
					VDP_drawBitmapEx(BG_A, &sgdk_logo_image, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 1, 3, FALSE);
					XGM2_setFMVolume(100);
					XGM2_play(xgm2_title);
					XGM2_fadeIn(120);
				}
			}
			else if (title_counter == 1) // press start screen
			{

				if (global_counter == 1)
				{
					clear_graphics(FALSE);

					VDP_drawBitmapEx(BG_A, &title_screen, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, FALSE);
				}
				else if (global_counter == 2)
				{
					VDP_drawText("PRESS START!", 10, 7);
				}
				if (title_skip)
				{
					title_skip = false;
					// start by cleaning up old level memory
					VDP_clearTileMap(BG_A, 0, 200, TRUE);
					VDP_clearTileMapRect(BG_A, 0, 0, 32, 28);
					title_counter = 2;
					setRandomSeed(global_counter);
					global_counter = 0; // reset this to 0 to time the next section
				}
				if (global_counter == 122)
				{
					VDP_clearText(10, 7, 12);
				}
				else if (global_counter == 182)
				{
					global_counter = 1;
				}
			}
			else if (title_counter == 2)
			{
				// game start transition
				game_state = GAME_STATE_TRANSITION;
				XGM2_stop();
				global_counter = 0;
			}
		}
		else if (game_state == GAME_STATE_OVER)
		{
			if (title_skip)
			{
				global_counter = 600; // skip to reset
			}

			if (global_counter == 1)
			{
				clear_graphics(TRUE);
				title_counter = 0; // make sure this is reset
				title_skip = false;
				XGM2_stop();
				XGM2_setFMVolume(100);
				XGM2_play(xgm2_over);
				VDP_drawBitmapEx(BG_A, &over_screen, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, FALSE);
			}
			else if (!title_skip)
			{
				char buffer[8];
				sprintf(buffer, "SCORE: %d", score);
				VDP_drawText(buffer, 11, 18);
				char buffer2[12];
				sprintf(buffer2, "DOORS SEALED: %d", total_doors_closed);
				VDP_drawText(buffer2, 8, 20);
			}
			else
			{
				global_counter = 0;
				game_state = GAME_STATE_TITLE;
				title_skip = false;
				clear_graphics(TRUE);
				SPR_end();
				SYS_hardReset();
			}
		}
		else if (game_state == GAME_STATE_TRANSITION)
		{

			if (global_counter == 241 || title_skip)
			{
				if (current_level == 0)
				{
					initPlayer();
				}
				// game start
				game_state = GAME_STATE_GAME;
				global_counter = 1;
				initLevel(current_level + 1);
				title_skip = false;
			}
			else if (global_counter == 1)
			{
				SYS_disableInts();
				SPR_clear();
				VDP_loadTileSet(&blank_tileset, 0, DMA);
				VDP_setTileMapEx(BG_B, &blank, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, 0), 0, 0, 0, 0, 32, 28, DMA);
				SYS_enableInts();
				XGM2_setFMVolume(60);
				XGM2_play(xgm2_level0);
				if (current_level == 0)
				{
					VDP_drawText("THE TIME IS", 2 + 5, 1 + 6);
					VDP_drawText("TOO EARLY;", 3 + 5, 2 + 6);
					VDP_drawText("THE THIRD STAR", 4 + 5, 4 + 6);
					VDP_drawText("DOTH NOT SHINE", 5 + 5, 5 + 6);
					VDP_drawText("Press A", 8 + 5, 8 + 6);
					VDP_drawText("to seal", 9 + 5, 9 + 6);
					VDP_drawText("the DOORS", 10 + 5, 10 + 6);
				}
				else if (current_level == 1) // level id - 1
				{
					VDP_drawText("FIND THE KEY...", 2 + 5, 3 + 6);
					VDP_drawText("TO SEAL THIS DOOR", 3 + 5, 5 + 6);
					// VDP_drawText("THE THIRD STAR", 4 + 5, 4 + 6);
					// VDP_drawText("DOTH NOT SHINE", 5 + 5, 5 + 6);
					// VDP_drawText("...SEAL...", 6 + 5, 8 + 6);
					// VDP_drawText("...THE...", 7 + 5, 9 + 6);
					// VDP_drawText("...DOORS...", 8 + 5, 10 + 6);
				}
				else if (current_level == 2)
				{
					VDP_drawText("THE TIME IS", 2 + 5, 1 + 6);
					VDP_drawText("TOO EARLY;", 3 + 5, 2 + 6);
					VDP_drawText("THE THIRD STAR", 4 + 5, 4 + 6);
					VDP_drawText("DOTH NOT SHINE", 5 + 5, 5 + 6);
					VDP_drawText("...SEAL...", 6 + 5, 8 + 6);
					VDP_drawText("...THE...", 7 + 5, 9 + 6);
					VDP_drawText("...DOORS...", 8 + 5, 10 + 6);
				}
				else if (current_level == 3)
				{
					VDP_drawText("THE TIME IS", 2 + 5, 1 + 6);
					VDP_drawText("TOO EARLY;", 3 + 5, 2 + 6);
					VDP_drawText("THE THIRD STAR", 4 + 5, 4 + 6);
					VDP_drawText("DOTH NOT SHINE", 5 + 5, 5 + 6);
					VDP_drawText("...SEAL...", 6 + 5, 8 + 6);
					VDP_drawText("...THE...", 7 + 5, 9 + 6);
					VDP_drawText("...DOORS...", 8 + 5, 10 + 6);
				}
			}
		}
		SYS_doVBlankProcess();
	}

	return 0;
}

void inGameJoyEvent(u16 joy, u16 changed, u16 state)
{
	input.joy = joy;
	input.changed = changed;
	input.state = state;
	checkInput(); // in the player.c file
}

void display_stats()
{
	char free_mem[16] = "MEM: ";
	char free_vram[16] = "VRAM: ";
	VDP_showCPULoad(12, 25);
	intToStr(MEM_getAllocated(), &free_mem[5], 1);
	VDP_drawText(free_mem, 1, 3);
	intToStr(SPR_getFreeVRAM(), &free_vram[6], 1);
	VDP_drawText(free_vram, 1, 4);
}