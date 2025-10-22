#include "player.h"
#include "globals.h"
#include "resources.h"
#include "level.h"
#include "enemy.h"
#include "spell.h"

void initSpell(u8 spell_type, fix16 x, fix16 y)
{
    if (spell_type == SPELL_CROSS)
    {
        initPlayerBullet(x, y, 0, 1);
    }
}

struct bulletData player_bullet_array[MAX_BULLETS];

void initPlayerBullet(fix16 x, fix16 y, f16 x_velocity, f16 y_velocity)
{
    u8 i = 0;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (!player_bullet_array[i].data.active)
        {
            struct bulletData bul;
            bul.data.sprite = SPR_addSprite(&bullet_sprite, x, y, TILE_ATTR(PAL0, 0, FALSE, FALSE));
            bul.data.x = x;
            bul.data.y = y;
            bul.data.active = true;
            bul.velocity.x = x_velocity;
            bul.velocity.y = y_velocity;
            SPR_setAnim(bul.data.sprite, 0);
            player_bullet_array[i] = bul;
            break;
        }
    }
}