PALETTE palette_0 "assets/gfx/PAL0.png"
PALETTE palette_1 "assets/gfx/PAL1.png"
PALETTE palette_2 "assets/gfx/PAL2.png"
PALETTE palette_3 "assets/gfx/PAL3.png"
SPRITE player_sprite "assets/gfx/player.png" 1 2 FAST 20
SPRITE door_sprite "assets/gfx/door.png" 1 2 FAST 30
SPRITE doorbeast_sprite "assets/gfx/doorbeast.png" 1 2 FAST 15
SPRITE demon_sprite "assets/gfx/demon.png" 1 2 FAST 10
SPRITE eye_sprite "assets/gfx/eye.png" 2 1 FAST 10
SPRITE eye_boss_sprite "assets/gfx/eye_boss.png" 4 2 FAST 8
SPRITE cultist_sprite "assets/gfx/cultist.png" 1 2 FAST 20
SPRITE bullet_sprite "assets/gfx/bullet.png" 1 1 FAST 10
SPRITE idol_sprite "assets/gfx/idol.png" 1 1 FAST 30
SPRITE key_sprite "assets/gfx/key.png" 1 1 FAST 30
SPRITE portal_sprite "assets/gfx/portal.png" 1 1 FAST 15
SPRITE secret_left_sprite "assets/gfx/secret_block_left.png" 1 1 FAST
SPRITE secret_down_sprite "assets/gfx/secret_block_down.png" 1 1 FAST
SPRITE secret_right_sprite "assets/gfx/secret_block_right.png" 1 1 FAST
SPRITE sacred_ground_sprite "assets/gfx/sacred_ground.png" 2 2 FAST 5
TILESET level_tileset "assets/gfx/tileset2.png" NONE NONE
TILEMAP level_1_map "assets/tiles/level_1.tmx" "Tile Layer 1" FAST FAST  0
TILEMAP level_2_map "assets/tiles/level_2.tmx" "Tile Layer 1" FAST FAST  0
TILEMAP level_3_map "assets/tiles/level_3.tmx" "Tile Layer 1" FAST FAST  0
TILEMAP level_4_map "assets/tiles/level_4.tmx" "Tile Layer 1" FAST FAST  0
TILEMAP level_5_map "assets/tiles/level_5.tmx" "Tile Layer 1" FAST FAST  0
TILEMAP level_6_map "assets/tiles/level_6.tmx" "Tile Layer 1" FAST FAST  0
TILEMAP level_7_map "assets/tiles/level_7.tmx" "Tile Layer 1" FAST FAST  0
//TILEMAP level_8_map "assets/tiles/level_8.tmx" "Tile Layer 1" FAST FAST  0
//TILEMAP level_9_map "assets/tiles/level_9.tmx" "Tile Layer 1" FAST FAST  0
//TILEMAP level_10_map "assets/tiles/level_10.tmx" "Tile Layer 1" FAST FAST  0
//TILEMAP level_q_map "assets/tiles/level_q.tmx" "Tile Layer 1" FAST FAST  0
TILESET border_tileset "assets/gfx/border_with_window.png" FAST ALL
TILESET border_transition_tileset "assets/gfx/border.png" FAST ALL
TILESET blank_tileset "assets/gfx/blank_screen.png" FAST ALL
TILEMAP border_image "assets/gfx/border_with_window.png" border_tileset
TILEMAP border_image_transition "assets/gfx/border.png" border_transition_tileset
TILEMAP blank "assets/gfx/blank_screen.png" blank_tileset
BITMAP sgdk_logo_image "res_inc/image/sgdk_logo_2.png" FAST
BITMAP title_screen "assets/gfx/title_screen.png" FAST
BITMAP over_screen "assets/gfx/over_screen.png" FAST
XGM2 xgm2_title "assets/music/title2.vgm"
XGM2 xgm2_level0 "assets/music/level.vgm"
XGM2 xgm2_over "assets/music/over.vgm"
WAV wav_shot "assets/sound/laser gun 4.wav" XGM2 6650
WAV wav_en_shot "assets/sound/laser gun 6.wav" XGM2 6650
WAV wav_player_hurt "assets/sound/body hit with grunt 1.wav" XGM2 6650
WAV wav_en_hurt "assets/sound/body hit with grunt 1.wav" XGM2 6650
WAV wav_door_beastmode "assets/sound/alien computer 2.wav" XGM2 6650
WAV wav_sacred "assets/sound/metal hit 2.wav" XGM2 6650
WAV wav_door_sealed "assets/sound/wood and metal cabinet closed 2.wav" XGM2 6650
//WAV wav_en_death "assets/sound/alien machine 10.wav" XGM2 6650
WAV wav_en_death "assets/sound/goo 3.wav" XGM2 6650
WAV wav_en_spawn "assets/sound/bug people 14.wav" XGM2 6650