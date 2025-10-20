#include <genesis.h>
#include "resources.h"
#include "globals.h"
#include "player.h"
#include "level.h"

void inGameJoyEvent(u16 joy, u16 changed, u16 state);

int main()
{
	VDP_setScreenWidth256();
	SPR_init();
	JOY_init();

	PAL_setPalette(PAL0, palette_1.data, DMA);
	PAL_setPalette(PAL1, palette_1.data, DMA);
	PAL_setPalette(PAL2, palette_2.data, DMA);
	VDP_setBackgroundColor(4);
	// VDP_setTextPalette(PAL0);

	initPlayer();
	door_array[0] = initObject(0, 140, 100);
	JOY_setEventHandler(inGameJoyEvent);

	// move to new header
	VDP_loadTileSet(&level_tileset, 0, DMA);
	VDP_setTileMapEx(BG_B, &level_map, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 0), 0, 0, 0, 0, 32, 28, DMA);
	// initLevel

	// bga = MAP_create(&level_map, TILEMAP_PLANE, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, VDPTilesFilled));

	// Update the number of tiles filled in order to avoid overlaping them when loading more
	// VDPTilesFilled += level_tileset.numTile;

	while (TRUE)
	{
		updatePlayer();
		updateLevel();
		SPR_update();
		VDP_drawText("test", 0, 0);
		SYS_doVBlankProcess();
		// VDP_waitVSync();
	}

	return 0;
}

void inGameJoyEvent(u16 joy, u16 changed, u16 state)
{
	input.joy = joy;
	input.changed = changed;
	input.state = state;
	checkInput();
}
