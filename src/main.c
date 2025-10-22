#include <genesis.h>
#include "resources.h"
#include "globals.h"
#include "player.h"
#include "level.h"
#include "enemy.h"
#include "game.h"

void inGameJoyEvent(u16 joy, u16 changed, u16 state);
void display_stats();

int main()
{
	VDP_setScreenWidth256();
	SPR_init();
	JOY_init();
	JOY_setEventHandler(inGameJoyEvent);
	VDP_setTextPalette(PAL3);
	VDP_setBackgroundColor(5); // change this per level?

	while (TRUE)
	{
		global_counter += 1; // update this counter no matter what

		if (game_state == GAME_STATE_GAME)
		{
			if (global_counter == 1)
			{
				VDP_clearTileMap(BG_A, 0, 200, TRUE);
				// VDP_clearTileMap(BG_B, 0, 200, TRUE);
				VDP_clearTileMapRect(BG_A, 0, 0, 32, 28);
				// load title screen 1 into vram and display
				PAL_setPalette(PAL0, palette_1.data, DMA);
				PAL_setPalette(PAL1, palette_1.data, DMA);
				PAL_setPalette(PAL2, palette_1.data, DMA);
				PAL_setPalette(PAL3, palette_3.data, DMA);
				initPlayer();
				initLevel(0);
			}
			else
			{
				updatePlayer();
				updateGame(global_counter);
				SPR_update();
				// debug
				debug_player_info_print();
				display_stats();

				SYS_doVBlankProcess();
			}
			if (global_counter >= 20000)
			{
				global_counter = 2;
			}
		}
		else if (game_state == GAME_STATE_PAUSE)
		{
			SYS_doVBlankProcess();
		}
		else if (game_state == GAME_STATE_TRANSITION)
		{
			SYS_doVBlankProcess();
		}
		else if (game_state == GAME_STATE_TITLE)
		{

			if (title_counter == 0)
			{

				if (global_counter >= 300 || title_skip) // time in frames for first title drop
				{
					title_skip = false;
					// start by cleaning up old level memory
					VDP_clearTileMap(BG_A, 0, 200, TRUE);
					// VDP_clearTileMap(BG_B, 0, 200, TRUE);
					VDP_clearTileMapRect(BG_A, 0, 0, 32, 28);
					title_counter += 1;
					global_counter = 0; // reset this to 0 to time the next section
				}
				else if (global_counter == 1)
				{
					// load title screen 0 into vram and display
					// PAL_setPalette(PAL3, palette_3.data, DMA);
					// SPR_init();
					VDP_drawBitmapEx(BG_A, &sgdk_logo_image, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 1, 3, FALSE);
					XGM2_setFMVolume(100);
					XGM2_play(xgm2_title);
					XGM2_fadeIn(15);
				}
			}
			else if (title_counter == 1)
			{

				if (global_counter == 1)
				{
					VDP_clearTileMap(BG_A, 0, 200, TRUE);
					// VDP_clearTileMap(BG_B, 0, 200, TRUE);
					VDP_clearTileMapRect(BG_A, 0, 0, 32, 28);
					// load title screen 1 into vram and display
					PAL_setPalette(PAL0, palette_1.data, DMA);
					PAL_setPalette(PAL1, palette_1.data, DMA);
					PAL_setPalette(PAL2, palette_1.data, DMA);
					PAL_setPalette(PAL3, palette_3.data, DMA);
					VDP_drawBitmapEx(BG_A, &title_screen, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, FALSE);

					// VDP_setTextPriority(1);
					// VDP_drawText("PRESS START TO PLAY", 10 + 9, 9 + 12);
				}
				if (title_skip)
				{
					title_skip = false;
					// start by cleaning up old level memory
					VDP_clearTileMap(BG_A, 0, 200, TRUE);
					// VDP_clearTileMap(BG_B, 0, 200, TRUE);
					VDP_clearTileMapRect(BG_A, 0, 0, 32, 28);
					title_counter = 2;
					global_counter = 0; // reset this to 0 to time the next section
				}
				if (global_counter == 300)
				{
					global_counter = 2;
					// VDP_drawBitmapEx(BG_A, &title_screen, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0, FALSE);
				}
			}
			else if (title_counter == 2)
			{ // game start!
				game_state = GAME_STATE_GAME;
				// XGM2_fadeOutAndStop(3);
				XGM2_stop();

				global_counter = 0;
			}
			SYS_doVBlankProcess();
		}
		else if (game_state == GAME_STATE_OVER)
		{

			game_state = GAME_STATE_TITLE;
			title_counter = 0;
			global_counter = 0;
			title_skip = false;
			// start by cleaning up old level memory
			VDP_clearTileMap(BG_A, 0, 400, TRUE);
			VDP_clearTileMap(BG_B, 0, 400, TRUE);
			// VDP_clearTileMap(BG_B, 0, 200, TRUE);
			VDP_clearTileMapRect(BG_A, 0, 0, 32, 28);
			VDP_clearTileMapRect(BG_B, 0, 0, 32, 28);
			// XGM2_fadeOutAndStop(120);
			XGM2_stop();
			SPR_clear();
			SPR_reset();
			SPR_defragVRAM();
			// SPR_end();
			SYS_doVBlankProcess();
		}
		else if (game_state == GAME_STATE_WIN)
		{
			SYS_doVBlankProcess();
		}
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
	VDP_showCPULoad(0, 0);
	intToStr(MEM_getAllocated(), &free_mem[5], 1);
	VDP_drawText(free_mem, 1, 3);
	intToStr(SPR_getFreeVRAM(), &free_vram[6], 1);
	VDP_drawText(free_vram, 1, 4);
}