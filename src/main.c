#include <genesis.h>
#include "resources.h"

int main()
{
	VDP_setScreenWidth256();
	VDP_drawText("Hello World!", 4, 4);

	PAL_setPalette(PAL1, moon.palette->data, DMA);
	VDP_setBackgroundColor(8);
	VDP_drawImageEx(BG_A, &moon, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 12, 12, 0, CPU);

	while (TRUE)
	{
		VDP_waitVSync();
	}
	return 0;
}
