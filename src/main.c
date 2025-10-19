#include <genesis.h>
#include "resources.h"
#include "player.h"

void inGameJoyEvent(u16 joy, u16 changed, u16 state);

int main()
{
	VDP_setScreenWidth256();
	SPR_init();
	JOY_init();

	PAL_setPalette(PAL0, palette_1.data, DMA);
	PAL_setPalette(PAL1, palette_1.data, DMA);
	VDP_setBackgroundColor(4);

	initPlayer();

	while (TRUE)
	{
		SPR_update();
		SYS_doVBlankProcess();
		// VDP_waitVSync();
	}

	return 0;
}

// void inGameJoyEvent(u16 joy, u16 changed, u16 state)
// {
// 	input.joy = joy;
// 	input.changed = changed;
// 	input.state = state;

// 	playerInputChanged();
// }