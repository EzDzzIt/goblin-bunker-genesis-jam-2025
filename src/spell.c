#include "player.h"
#include "globals.h"
#include "resources.h"
#include "level.h"
#include "enemy.h"
#include "spell.h"

void initSpell(u8 spell_type, fix16 x, fix16 y)
{
    if (spell_type == SPELL_SHOT)
    {

        initPlayerBullet(x, y, 0, 1);
        initPlayerBullet(x, y, 0, -1);
        initPlayerBullet(x, y, -1, 0);
        initPlayerBullet(x, y, 1, 0);
    }
    else if (spell_type == SPELL_SACRED)
    {
        initSacredGround(x - 4, y + 8);
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

struct spellData sacred_ground_array[MAX_SACRED_GROUND];

void initSacredGround(fix16 x, fix16 y)
{

    u8 i = 0;
    for (i = 0; i < MAX_SACRED_GROUND; i++)
    {
        if (!sacred_ground_array[i].data.active)
        {
            struct spellData spell;
            spell.data.sprite = SPR_addSprite(&sacred_ground_sprite, x, y, TILE_ATTR(PAL0, 0, FALSE, FALSE));
            SPR_setAlwaysAtBottom(spell.data.sprite);
            spell.data.x = x;
            spell.data.y = y;

            spell.data.active = true;
            spell.lifetime = 60;
            SPR_setAnim(spell.data.sprite, SACRED_GROUND_ANIM);
            sacred_ground_array[i] = spell;
            break;
        }
    }
}

void updateSpells()
{
    u8 i = 0;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (player_bullet_array[i].data.active)
        {
            // bullet updates
            player_bullet_array[i].data.x += player_bullet_array[i].velocity.x;
            player_bullet_array[i].data.y += player_bullet_array[i].velocity.y;
            SPR_setPosition(player_bullet_array[i].data.sprite, player_bullet_array[i].data.x, player_bullet_array[i].data.y);
            bool collided = false;
            u8 j = 0;
            // collide with doors??? or no?
            // for (j = 0; j < MAX_DOORS; j++)
            // {
            //     if (door_array[j].data.active)
            //     {
            //         if (collision_check(player_bullet_array[i].data.x, player_bullet_array[i].data.y, BULLET_WIDTH, BULLET_HEIGHT, door_array[j].data.x, door_array[j].data.y, DOOR_WIDTH, DOOR_HEIGHT))
            //         {
            //             SPR_setPalette(door_array[j].data.sprite, PAL3);
            //             collided = true;
            //             break;
            //         }
            //     }
            // }
            for (j = 0; j < MAX_ENEMIES; j++)
            {
                if (enemy_array[j].data.active)
                {
                    if (enemy_array[j].hurt_cooldown <= 0)
                    {
                        if (collision_check(player_bullet_array[i].data.x, player_bullet_array[i].data.y, BULLET_WIDTH, BULLET_HEIGHT, enemy_array[j].data.x, enemy_array[j].data.y, enemy_array[j].width, enemy_array[j].height))
                        {
                            enemy_array[j].hurt_cooldown = 20;
                            enemy_array[j].hp -= 1;
                            collided = true;
                            break;
                        }
                    }
                }
            }
            if (collided || UPDATE_SCROLL)
            {

                SPR_releaseSprite(player_bullet_array[i].data.sprite);
                player_bullet_array[i].data.active = false;
            }
            else
            {
                if (player_bullet_array[i].data.x > SCREEN_X_END)
                {
                    SPR_releaseSprite(player_bullet_array[i].data.sprite);
                    player_bullet_array[i].data.active = false;
                }
                else if (player_bullet_array[i].data.x < SCREEN_X_OFFSET)
                {
                    SPR_releaseSprite(player_bullet_array[i].data.sprite);
                    player_bullet_array[i].data.active = false;
                }
                if (player_bullet_array[i].data.y > SCREEN_Y_END - BULLET_HEIGHT)
                {
                    SPR_releaseSprite(player_bullet_array[i].data.sprite);
                    player_bullet_array[i].data.active = false;
                }
                else if (player_bullet_array[i].data.y < SCREEN_Y_OFFSET)
                {
                    SPR_releaseSprite(player_bullet_array[i].data.sprite);
                    player_bullet_array[i].data.active = false;
                }
            }
        }
    }
    // update sacred ground
    for (i = 0; i < MAX_SACRED_GROUND; i++)
    {
        if (sacred_ground_array[i].lifetime > 0)
        {
            sacred_ground_array[i].lifetime -= 1;
            if (sacred_ground_array[i].lifetime == 0)
            {
                sacred_ground_array[i].data.active = false;
                SPR_releaseSprite(sacred_ground_array[i].data.sprite);
                continue;
            }
            if (sacred_ground_array[i].data.active)
            {
                if (!UPDATE_SCROLL)
                {
                    u8 j = 0;
                    for (j = 0; j < MAX_DOORS; j++)
                    {
                        // collision check between sacred ground and doors needs slight mod to work
                        if (door_array[j].beastmode)
                        {
                            if (collision_check(sacred_ground_array[i].data.x - 2, sacred_ground_array[i].data.y - 3, SACRED_GROUND_WIDTH + 2, SACRED_GROUND_HEIGHT + 3, door_array[j].data.x, door_array[j].data.y, DOOR_WIDTH, DOOR_HEIGHT))
                            {
                                if (door_array[j].close_cooldown == 0)
                                {
                                    shutDoor(j);
                                }
                            }
                        }
                    }
                }
                else
                {
                    sacred_ground_array[i].data.active = false;
                    SPR_releaseSprite(sacred_ground_array[i].data.sprite);
                }
            }
        }
    }
}

void initWarp()
{
    player.warp_cooldown = PLAYER_WARP_COOLDOWN;
    SPR_setAnim(player.sprite, PLAYER_ANIM_WARP);
    player.x = ((random() % (20 - 1 + 1)) + 1) * 8;
    player.y = ((random() % (16 - 1 + 1)) + 1) * 8;
}