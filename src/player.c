#include "player.h"
// #include "global.h"
// #include "map.h"
#include "resources.h"

struct playerData player;

void initPlayer()
{
    player.sprite = SPR_addSprite(&player_sprite, 0, 0, TILE_ATTR(PAL0, 0, FALSE, FALSE));
}