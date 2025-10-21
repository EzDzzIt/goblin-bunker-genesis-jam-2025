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

	PAL_setPalette(PAL0, palette_1.data, DMA);
	PAL_setPalette(PAL1, palette_1.data, DMA);
	PAL_setPalette(PAL2, palette_1.data, DMA);
	PAL_setPalette(PAL3, palette_3.data, DMA);

	VDP_setTextPalette(PAL3);

	VDP_setBackgroundColor(5); // change this per level?

	// initialize some global vars
	u16 global_counter = 0;
	// track which title screen to show
	u8 title_counter = 0;
	enum game_state_enum game_state = GAME_STATE_GAME;
	initPlayer();
	initLevel(0);

	while (TRUE)
	{
		if (game_state == GAME_STATE_GAME)
		{
			global_counter += 1;
			updatePlayer(global_counter);
			updateGame(global_counter);
			SPR_update();

			debug_player_info_print();

			// display_stats();
			SYS_doVBlankProcess();
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
			global_counter += 1;
			if (title_counter == 0)
			{
				if (global_counter >= 180)
				{
					title_counter += 1;
					global_counter = 0; // reset this to 0 to time the next section
				}
				else if (global_counter == 1)
				{
					// load title screen 0 into vram and display
				}
			}
			else if (title_counter == 1)
			{
				if (global_counter >= 60 * 15)
				{ // wait for 15 seconds, then reset
					title_counter = 0;
					global_counter = 0;
				}
				else if (global_counter == 1)
				{
					// load title screen 1 into vram and display
				}
			}
			SYS_doVBlankProcess();
		}
		else if (game_state == GAME_STATE_OVER)
		{
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