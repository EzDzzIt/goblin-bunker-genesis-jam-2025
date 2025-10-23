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
        initSacredGround(x - 4, y + 8);
        initPlayerBullet(x, y, 0, 1);
        initPlayerBullet(x, y, 0, -1);
        initPlayerBullet(x, y, -1, 0);
        initPlayerBullet(x, y, 1, 0);
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
            bul.velocity.x = x_velocity * PLAYER_BULLET_SPEED_0;
            bul.velocity.y = y_velocity * PLAYER_BULLET_SPEED_0;
            SPR_setAnim(bul.data.sprite, 0);
            player_bullet_array[i] = bul;
            break;
        }
    }
}

struct spellData sacred_ground_array[2];

void initSacredGround(fix16 x, fix16 y)
{

    u8 i = 0;
    for (i = 0; i < 2; i++)
    {
        if (!sacred_ground_array[i].data.active)
        {
            struct spellData spell;
            spell.data.sprite = SPR_addSprite(&sacred_ground_sprite, x, y, TILE_ATTR(PAL0, 0, FALSE, FALSE));
            SPR_setAlwaysAtBottom(spell.data.sprite);
            spell.data.x = x;
            spell.data.y = y;
            spell.data.active = true;
            spell.lifetime = 120;
            SPR_setAnim(spell.data.sprite, SACRED_GROUND_ANIM);
            sacred_ground_array[i] = spell;
            break;
        }
    }
}