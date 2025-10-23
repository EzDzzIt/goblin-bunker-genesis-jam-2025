PALETTE palette_0 "assets/PAL0.png"
PALETTE palette_1 "assets/PAL1.png"
PALETTE palette_2 "assets/PAL2.png"
PALETTE palette_3 "assets/PAL3.png"
SPRITE player_sprite "assets/player.png" 1 2 FAST 20
SPRITE door_sprite "assets/door.png" 1 2 FAST 30
SPRITE doorbeast_sprite "assets/doorbeast.png" 1 2 FAST 15
SPRITE demon_sprite "assets/demon.png" 1 2 FAST 10
SPRITE bullet_sprite "assets/bullet.png" 1 1 FAST 10
SPRITE sacred_ground_sprite "assets/sacred_ground.png" 2 2 FAST 5
TILESET level_tileset "assets/tileset.png" FAST NONE 
TILEMAP level_map "assets/tiled/level0_small.tmx" "Tile Layer 1" FAST FAST  0
TILESET border_tileset "assets/border.png" FAST ALL
TILEMAP border_image "assets/border.png" border_tileset
BITMAP sgdk_logo_image "res_inc/image/sgdk_logo_2.png" FAST
BITMAP title_screen "assets/title_screen.png" FAST
BITMAP over_screen "assets/over_screen.png" FAST
XGM2 xgm2_title "assets/music/title.vgm"
XGM2 xgm2_level0 "assets/music/level0.vgm"