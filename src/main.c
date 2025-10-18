#include <genesis.h>
#include "resources.h"

int main()
{
	VDP_setScreenWidth256();

	PAL_setPalette(PAL0, palette_1.data, DMA);
	PAL_setPalette(PAL1, palette_1.data, DMA);
	VDP_setBackgroundColor(4);

	SPR_init();
	Sprite *testing = SPR_addSprite(&player, 0, 0, TILE_ATTR(PAL0, 0, FALSE, FALSE));
	// SPR_setVisibility(testing, true);
	// VDP_drawText("Hello World!", 4, 4);

	while (TRUE)
	{
		SPR_update();
		SYS_doVBlankProcess();
		VDP_waitVSync();
	}

	return 0;
}
