#include <genesis.h>
#include "resources.h"
#include "player.h"
#include "globals.h"

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

	// move to new header
	VDP_loadTileSet(&level_tileset, 0, DMA);
	VDP_setTileMap(BG_B, &level_map, 0, 0, 32, 28, DMA);
	// bga = MAP_create(&level_map, TILEMAP_PLANE, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, VDPTilesFilled));

	// Update the number of tiles filled in order to avoid overlaping them when loading more
	// VDPTilesFilled += level_tileset.numTile;

	while (TRUE)
	{
		updatePlayer();
		SPR_update();
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
}
