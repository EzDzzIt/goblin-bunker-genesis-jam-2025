#include <genesis.h>
#include "resources.h"

int main()
{
	VDP_setScreenWidth256();
	VDP_drawText("Hello World!", 4, 4);

	// Define a custom palette with 16 colors
	const u16 my_custom_palette[16] = {
		RGB24_TO_VDPCOLOR(0x000000), // Index 0: Transparent black
		RGB24_TO_VDPCOLOR(0xFF0000), // Index 1: Red
		RGB24_TO_VDPCOLOR(0x00FF00), // Index 2: Green
		RGB24_TO_VDPCOLOR(0x0000FF), // Index 3: Blue
		RGB24_TO_VDPCOLOR(0xFFFF00), // Index 4: Yellow
		// ... add 11 more custom colors
		RGB24_TO_VDPCOLOR(0xFFFFFF), // Index 15: White (for text)
	};

	PAL_setPalette(PAL1, my_custom_palette, DMA);
	VDP_setBackgroundColor(8);
	VDP_drawImageEx(BG_A, &player, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 12, 12, 0, CPU);

	while (TRUE)
	{
		VDP_waitVSync();
	}
	return 0;
}
