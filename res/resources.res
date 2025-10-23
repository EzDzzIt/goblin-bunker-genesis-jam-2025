PALETTE palette_0 "assets/gfx/PAL0.png"
PALETTE palette_1 "assets/gfx/PAL1.png"
PALETTE palette_2 "assets/gfx/PAL2.png"
PALETTE palette_3 "assets/gfx/PAL3.png"
SPRITE player_sprite "assets/gfx/player.png" 1 2 FAST 20
SPRITE door_sprite "assets/gfx/door.png" 1 2 FAST 30
SPRITE doorbeast_sprite "assets/gfx/doorbeast.png" 1 2 FAST 15
SPRITE demon_sprite "assets/gfx/demon.png" 1 2 FAST 10
SPRITE bullet_sprite "assets/gfx/bullet.png" 1 1 FAST 10
SPRITE sacred_ground_sprite "assets/gfx/sacred_ground.png" 2 2 FAST 5
TILESET level_tileset "assets/gfx/tileset.png" FAST NONE 
TILEMAP level_map "assets/tiled/level0_small.tmx" "Tile Layer 1" FAST FAST  0
TILESET border_tileset "assets/gfx/border.png" FAST ALL
TILEMAP border_image "assets/gfx/border.png" border_tileset
BITMAP sgdk_logo_image "res_inc/image/sgdk_logo_2.png" FAST
BITMAP title_screen "assets/gfx/title_screen.png" FAST
BITMAP over_screen "assets/gfx/over_screen.png" FAST
XGM2 xgm2_title "assets/music/title.vgm"
XGM2 xgm2_level0 "assets/music/level0.vgm"