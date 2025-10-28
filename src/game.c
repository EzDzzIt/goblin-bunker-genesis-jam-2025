#include "level.h"
#include "globals.h"
#include "resources.h"
#include "enemy.h"
#include "player.h"
#include "spell.h"

// game logic

void updateGame()
{

    if (global_counter % 120 == 0)
    {
        // score += 1;
        SPR_defragVRAM();
    }

    // update doors
    updateDoors();
    // update objects
    updateObjects();
    // update enemies
    updateEnemies();
    // update enemy bullets
    updateEnemyBullets();
    // update player spells and projectiles
    updateSpells();
}
